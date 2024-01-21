// DisplayManager.cpp
#include "DisplayManager.h"
#include "Logger.h"
#include <iomanip> // for std::setprecision
#include <sstream>

DisplayManager::DisplayManager(
    std::shared_ptr<EventManager> eventManager,
    std::shared_ptr<OilChangeTracker> oilChangeTracker,
    std::map<std::string, std::shared_ptr<IDisplay>> &displays)
    : _eventManager(eventManager), _oilChangeTracker(oilChangeTracker),
      _displays(displays) {

  auto primaryIt = _displays.find("Display1");
  if (primaryIt != _displays.end()) {
    _primaryDisplay = primaryIt->second;
  } else {
    Logger::info("[DisplayManager] Primary display not found");
  }
  // Only attempt to set the secondary display if 2 displays are given
  if (displays.size() > 1) {
    auto secondaryIt = _displays.find("Display2");
    if (secondaryIt != _displays.end()) {
      _secondaryDisplay = secondaryIt->second;
    } else {
      Logger::info("[DisplayManager] Secondary display not found");
    }
  }
  // If secondaryDisplayId is empty, _secondaryDisplay remains uninitialized
  // (nullptr)
}

DisplayManager::~DisplayManager() {
  _eventManager->unsubscribe(shared_from_this(), Event::Display);
}

void DisplayManager::initialize() {
  _eventManager->subscribe(shared_from_this(), Event::Display);
  _lastUpdate = std::chrono::steady_clock::now();
}

void DisplayManager::onNotify(Event type, Parameter parameter,
                              const std::string &message) {
  if (type == Event::Display) {
    if (parameter == Parameter::DisplayMessage && _primaryDisplay) {
      _primaryDisplay->displayMessage(message);
    } else if (parameter == Parameter::SecondaryDisplayMessage &&
               _secondaryDisplay) {
      _secondaryDisplay->displayMessage(message);
    }
  }
}

void DisplayManager::update() {
  auto now = std::chrono::steady_clock::now();
  if (now - _lastUpdate >=
      std::chrono::milliseconds(500)) { // Update every 500ms
    auto currentData = _oilChangeTracker->getCurrentData();
    std::string message = formatDisplayMessage(currentData);
    // Update displays
    if (_primaryDisplay) {
      _primaryDisplay->displayMessage(message);
    }
    _lastData = currentData;
    _lastUpdate = now;
  }
}

std::string DisplayManager::formatDisplayMessage(
    const OilChangeTracker::TrackerData &data) {
  std::stringstream ss;
  ss << std::fixed
     << std::setprecision(2); // Set fixed floating point with 2 decimal places

  // Append data
  ss << "FillCap:" << data.fillCapacity << ";"
     << "AmtFilled:" << data.amountFilled << ";"
     << "AmtExtracted:" << data.amountExtracted << ";"
     << "FillRate:" << data.fillFlowRate << ";"
     << "ExtractRate:" << data.extractFlowRate << ";"
     << "Voltage:" << data.voltage << ";"
     << "Temp:" << data.oilTemperature << ";"
     << "FillLPS:" << convertLPSState(data.fillLPSState) << ";"
     << "ExtractLPS:" << convertLPSState(data.extractLPSState);

  // Calculate checksum
  unsigned int checksum = calculateChecksum(ss.str());

  // Append checksum and end marker
  ss << ";" << checksum;

  return ss.str();
}

unsigned int DisplayManager::calculateChecksum(const std::string &message) {
  unsigned int sum = 0;
  for (char c : message) {
    sum += static_cast<unsigned int>(c);
  }
  return sum % 256; // Assuming a byte-size checksum
}

std::string DisplayManager::convertLPSState(int state) {
  return state == 0 ? "low" : "high";
}
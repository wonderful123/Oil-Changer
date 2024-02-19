// DisplayManager.cpp
#include "DisplayManager.h"
#include "Logger.h"
#include <cmath>
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
  auto currentData = _oilChangeTracker->getCurrentData();
  auto now = std::chrono::steady_clock::now();
  std::stringstream updateMessage;
  updateMessage << std::fixed << std::setprecision(2);

  // Evaluate each change for significance, except fillCapacity which triggers
  // an immediate update
  bool dataChanged = false;

  // Immediate update for fillCapacity
  if (_lastData.fillCapacity != currentData.fillCapacity) {
    updateMessage << "FillCap:" << currentData.fillCapacity << ";";
    dataChanged = true;
    _lastData.fillCapacity =
        currentData.fillCapacity; // Update this immediately to prevent
                                  // duplicate messages
  }

  if (dataChanged || now - _lastUpdate >= _updateInterval) {
    // Check other fields for significant changes
    dataChanged |=
        appendChange(updateMessage, "AmtFilled", _lastData.amountFilled,
                     currentData.amountFilled);
    dataChanged |=
        appendChange(updateMessage, "AmtExtracted", _lastData.amountExtracted,
                     currentData.amountExtracted);
    dataChanged |=
        appendChange(updateMessage, "FillFlow", _lastData.fillFlowRate,
                     currentData.fillFlowRate);
    dataChanged |=
        appendChange(updateMessage, "ExtractFlow", _lastData.extractFlowRate,
                     currentData.extractFlowRate);
    dataChanged |= appendChange(updateMessage, "Voltage", _lastData.voltage,
                                currentData.voltage);
    dataChanged |=
        appendChange(updateMessage, "OilTemp", _lastData.oilTemperature,
                     currentData.oilTemperature);
    dataChanged |=
        appendStateChange(updateMessage, "FillLPS", _lastData.fillLPSState,
                          currentData.fillLPSState);
    dataChanged |= appendStateChange(updateMessage, "ExtractLPS",
                                     _lastData.extractLPSState,
                                     currentData.extractLPSState);
    if (_lastData.currentState != currentData.currentState) {
      // If there's a state change, append it directly
      updateMessage << "State:" << currentData.currentState << ";";
      dataChanged = true; // Mark that there's a significant change
    }
  }

  // Only send the update if there's been a significant change or if it's
  // specifically for fillCapacity
  if (dataChanged) {
    // Finalize the update message
    std::string messageToSend = updateMessage.str();
    if (!messageToSend.empty()) {
      messageToSend +=
          "CKS:" + std::to_string(calculateChecksum(messageToSend));
      if (_primaryDisplay) {
        _primaryDisplay->displayMessage(messageToSend);
      }
      _lastData = currentData; // Update the last known state for all fields
      _lastUpdate = now;       // Reset the update timer
    }
  }
}

bool DisplayManager::appendChange(std::stringstream &ss, const std::string &key,
                                  double oldValue, double newValue,
                                  bool forceImmediate) {
  bool significantChange =
      forceImmediate || std::fabs(oldValue - newValue) > CHANGE_THRESHOLD;
  if (significantChange) {
    ss << key << ":" << newValue << ";";
    return true;
  }
  return false;
}

bool DisplayManager::appendChange(std::stringstream &ss, const std::string &key,
                                  const std::string &oldValue,
                                  const std::string &newValue,
                                  bool forceImmediate) {
  if (forceImmediate || oldValue != newValue) {
    ss << key << ":" << newValue << ";";
    return true;
  }
  return false;
}

// Special handling for state conversion to string, assuming it's an integer
// type
bool DisplayManager::appendStateChange(std::stringstream &ss,
                                       const std::string &key, int oldValue,
                                       int newValue) {
  if (oldValue != newValue) {
    ss << key << ":" << convertLPSState(newValue) << ";";
    return true;
  }
  return false;
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

bool DisplayManager::hasSignificantChange(double oldValue, double newValue,
                                          double threshold) {
  return std::fabs(oldValue - newValue) > threshold;
}
// BuzzerManager.cpp
#include "BuzzerManager.h"
#include "EventManager/EventManager.h"
#include "IBuzzer.h"
#include "Logger.h"
#include "SystemFactory.h"

// Note: This code is mostly redundant. The buzzer could be used directly.
// Kept because it may be used.

BuzzerManager::BuzzerManager(std::shared_ptr<IBuzzer> buzzer,
                             std::shared_ptr<InteractionSettings> &settings,
                             std::shared_ptr<EventManager> eventManager)
    : _buzzer(buzzer), _eventManager(eventManager) {
  if (_buzzer) {
    _buzzer->updateSettings(settings);
  } else {
    Logger::error(
        "[BuzzerManager] Buzzer component is not set or initialized.");
  }
}

void BuzzerManager::initialize() {
  // Subscribe to buzzer beep events
  _eventManager->subscribe(shared_from_this(), Event::Buzzer);
}

void BuzzerManager::onNotify(Event type, Parameter beepType) {
  if (beepType == Parameter::SingleBeep) {
    beep();
  } else if (beepType == Parameter::DoubleBeep) {
    doubleBeep();
  } else if (beepType == Parameter::RapidBeep) {
    rapidBeep();
  } else if (beepType == Parameter::StopBeep) {
    stop();
  }
}

void BuzzerManager::triggerBuzzer(int frequency, int duration) {
  if (_buzzer) {
    _buzzer->beep(frequency, duration);
  }
}

void BuzzerManager::beep() {
  if (_buzzer) {
    _buzzer->beep();
  }
}

void BuzzerManager::rapidBeep() {
  if (_buzzer) {
    _buzzer->rapidBeep();
  }
}

void BuzzerManager::doubleBeep() {
  if (_buzzer) {
    _buzzer->doubleBeep();
  }
}

void BuzzerManager::stop() {
  if (_buzzer) {
    _buzzer->stop();
  }
}

void BuzzerManager::updateSettings(
    std::shared_ptr<InteractionSettings> &settings) {
  if (_buzzer) {
    _buzzer->updateSettings(settings);
  }
}

void BuzzerManager::setOnRapidBeepCallback(std::function<void()> callback) {
  _buzzer->setOnRapidBeepCallback(callback);
}

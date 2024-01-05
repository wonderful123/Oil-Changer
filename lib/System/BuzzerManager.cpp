// BuzzerManager.cpp
#include "BuzzerManager.h"
#include "EventManager/EventManager.h"
#include "IBuzzer.h"
#include "Logger.h"
#include "SystemFactory.h"

// Note: This code is mostly redundant. The buzzer could be used directly.
// Kept because it may be used.

BuzzerManager::BuzzerManager(std::shared_ptr<IBuzzer> buzzer,
                             std::shared_ptr<InteractionSettings> &settings)
    : _buzzer(buzzer) {
  if (_buzzer) {
    _buzzer->updateSettings(settings);
  } else {
    Logger::error(
        "[BuzzerManager] Buzzer component is not set or initialized.");
  }

  // Subscribe to buzzer beep events
  SystemFactory::getInstance().getEventManager()->subscribe(
      std::shared_ptr<IEventListener>(this), EventType::BUZZER_BEEP);
}

void BuzzerManager::onNotify(EventType eventType, const EventData &eventData) {
  if (eventData.id == "beep") {
    beep();
  } else if (eventData.id == "double_beep") {
    doubleBeep();
  } else if (eventData.id == "rapid_beep") {
    rapidBeep();
  } else if (eventData.id == "stop") {
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

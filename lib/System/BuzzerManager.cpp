// BuzzerManager.cpp
#include "BuzzerManager.h"

BuzzerManager::BuzzerManager(std::shared_ptr<IMediator> mediator,
                             std::shared_ptr<IBuzzer> buzzer,
                             const InteractionSettings& settings)
    : IColleague(mediator), _buzzer(buzzer) {
  updateSettings(settings);
}

void BuzzerManager::triggerBuzzer(int frequency, int duration) {
  Logger::info("[BuzzerManager] Triggering buzzer");
  if (_buzzer) {
    Logger::info("[BuzzerManager] BEEP");
    _buzzer->beep();
    _buzzer->beep(frequency, duration);
  }
}

void BuzzerManager::updateSettings(const InteractionSettings& settings) {
  Logger::debug("[BuzzerManager] Updating settings");
  if (_buzzer) {
    _buzzer->updateSettings(settings);
  } else {
    Logger::error(
        "[BuzzerManager] Buzzer component is not set or initialized.");
  }
}

void BuzzerManager::receiveEvent(EventType eventType,
                                 const EventData* eventData) {
  if (eventType == BUTTON_PRESSED) {
    // Check specific button if needed and trigger buzzer
    Logger::info("[BuzzerManager] Button pressed - event received");
    //_buzzer->beep();
  }

  if (eventType == BUTTON_AUTO_REPEAT) {
    Logger::info("[BuzzerManager] Button auto repeat - event received");
    const auto& beepSettings = _settings.beepSettings;
    _buzzer->beep(beepSettings.standardFrequency,
                  beepSettings.standardDurationMs);
  }
}

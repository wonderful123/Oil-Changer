// BuzzerManager.cpp
#include "BuzzerManager.h"
#include "Logger.h"

BuzzerManager::BuzzerManager(std::shared_ptr<IBuzzer> buzzer,
                             std::shared_ptr<InteractionSettings> &settings)
    : _settings(settings), _buzzer(buzzer) {
  if (_buzzer) {
    _buzzer->updateSettings(settings);
  } else {
    Logger::error(
        "[BuzzerManager] Buzzer component is not set or initialized.");
  }
}

void BuzzerManager::triggerBuzzer(int frequency, int duration) {
  Logger::info("[BuzzerManager] Triggering buzzer");
  if (_buzzer) {
    Logger::info("[BuzzerManager] BEEP");
    _buzzer->beep(frequency, duration);
  }
}

void BuzzerManager::updateSettings(
    std::shared_ptr<InteractionSettings> &settings) {
  _buzzer->updateSettings(settings);
}

void BuzzerManager::onNotify(const std::string &event,
                             const std::string &buttonId) {
  if (event == "button_pressed") {
    // Trigger a single beep when the button is pressed
    _buzzer->beep();
  } else if (event == "button_auto_repeat") {
    // Trigger rapid beeping when button is held down
    startRapidBeep();
  } else if (event == "button_released") {
    // Stop rapid beeping when the button is released
    _buzzer->stop();
  }
}

void BuzzerManager::startRapidBeep() {
  uint rapidBeepFrequency = _settings->beepSettings.rapidBeepFrequency;
  uint rapidBeepDuration = _settings->beepSettings.rapidBeepDuration;
  uint rapidBeepPauseDuration = _settings->beepSettings.rapidBeepPauseDuration;
  _buzzer->rapidBeep(rapidBeepFrequency, rapidBeepDuration,
                     rapidBeepPauseDuration);
}
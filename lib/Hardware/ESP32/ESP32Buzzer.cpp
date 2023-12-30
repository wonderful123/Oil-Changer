#ifdef PLATFORM_ESP32

#include "ESP32Buzzer.h"

const uint DEFAULT_PWM_CHANNEL = 0;
const uint DEFAULT_PWM_RESOLUTION = 8;
const uint DEFAULT_PWM_FREQUENCY = 2713;

ESP32Buzzer::ESP32Buzzer(const HardwarePinConfig &config) : BuzzerBase(config) {
  ledcSetup(DEFAULT_PWM_CHANNEL, DEFAULT_PWM_FREQUENCY, DEFAULT_PWM_RESOLUTION);
  ledcAttachPin(config.pinNumber, DEFAULT_PWM_CHANNEL);
  setInitialized(true);
}

ESP32Buzzer::~ESP32Buzzer() {
  stop(); // Ensure the buzzer is stopped
}

void ESP32Buzzer::adjustVolume(float volume) {
  BuzzerBase::adjustVolume(volume);
  int dutyCycle = static_cast<int>(_volume * 255.0f);
  ledcWrite(DEFAULT_PWM_CHANNEL, dutyCycle);
  silenceBuzzer();
}

void ESP32Buzzer::beep(uint frequency, uint duration) {
  ledcWriteTone(DEFAULT_PWM_CHANNEL, frequency);
  _state.isBeeping = true;
  _beepTimer.once_ms(duration, handleBeepFinishCallback, this);
}

void ESP32Buzzer::handleBeepFinishCallback(ESP32Buzzer *buzzer) {
  buzzer->silenceBuzzer();
  buzzer->_state.isBeeping = false;
}

void ESP32Buzzer::doubleBeep(uint frequency, uint duration,
                             uint pauseDuration) {
  _settings.doubleBeepFrequency = frequency;
  _settings.doubleBeepDuration = duration;
  _state.isDoubleBeepSecondStage =
      false;                 // Indicates the first beep is being played
  beep(frequency, duration); // Start the first beep
  // Schedule the second beep after the first beep duration plus the pause
  // duration
  _rapidBeepTimer.once_ms(duration + pauseDuration,
                          &handleDoubleBeepTimerCallback, this);
}

void ESP32Buzzer::handleDoubleBeepTimerCallback(ESP32Buzzer *buzzer) {
  if (buzzer) {
    if (!buzzer->_state.isDoubleBeepSecondStage) {
      // Time for the second beep
      buzzer->_state.isDoubleBeepSecondStage = true;
      buzzer->beep(buzzer->_settings.standardFrequency,
                   buzzer->_settings.doubleBeepDuration);
    } else {
      // Second beep is done, stop beeping
      buzzer->stop();
    }
  }
}

void ESP32Buzzer::rapidBeep(uint frequency, uint duration, uint pauseInterval) {
  _settings.rapidBeepFrequency = frequency;
  _settings.rapidBeepDuration = duration;
  _settings.rapidBeepPauseDuration = pauseInterval;
  _state.isRapidBeeping = true;

  beep(frequency, duration);
  _onRapidBeepCallback();
  _rapidBeepTimer.once_ms(duration + pauseInterval,
                          &handleRapidBeepTimerCallback, this);
}

void ESP32Buzzer::handleRapidBeepTimerCallback(ESP32Buzzer *buzzer) {
  if (buzzer && buzzer->_state.isRapidBeeping) {
    buzzer->beep(buzzer->_settings.rapidBeepFrequency,
                 buzzer->_settings.rapidBeepDuration);
    buzzer->_onRapidBeepCallback();
    // Schedule the next beep after the current beep duration PLUS the pause
    // interval.
    buzzer->_rapidBeepTimer.once_ms(
        buzzer->_settings.rapidBeepDuration +
            buzzer->_settings.rapidBeepPauseDuration,
        handleRapidBeepTimerCallback, buzzer);
  }
}

void ESP32Buzzer::stop() {
  silenceBuzzer();     // Stop the buzzer sound instantly
  _beepTimer.detach(); // Detach the timer to stop callbacks
  _state.isBeeping = false;
  _state.isRapidBeeping = false;
  _rapidBeepTimer.detach();
}

void ESP32Buzzer::silenceBuzzer() { ledcWriteTone(DEFAULT_PWM_CHANNEL, 0); }

#endif //
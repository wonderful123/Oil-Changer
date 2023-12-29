#ifdef PLATFORM_ESP32

#include "ESP32Buzzer.h"

const uint DEFAULT_PWM_CHANNEL = 0;
const uint DEFAULT_PWM_RESOLUTION = 8;
const uint DEFAULT_PWM_FREQUENCY = 2713;

ESP32Buzzer::ESP32Buzzer(const HardwarePinConfig &config)
    : BuzzerBase(config) {
  ledcSetup(DEFAULT_PWM_CHANNEL, DEFAULT_PWM_FREQUENCY, DEFAULT_PWM_RESOLUTION);
  ledcAttachPin(config.pinNumber, DEFAULT_PWM_CHANNEL);
  setInitialized(true);
}

ESP32Buzzer::~ESP32Buzzer() {
  stop(); // Ensure the buzzer is stopped
}

void ESP32Buzzer::adjustVolume(float volume) {
  BuzzerBase::adjustVolume(volume); // Store the volume in the base class
  int dutyCycle = static_cast<int>(
      _volume * 255.0f); // Convert volume to duty cycle (0-255)
  ledcWrite(DEFAULT_PWM_CHANNEL,
            dutyCycle); // Adjust the duty cycle to change the volume
  silenceBuzzer();
}

void ESP32Buzzer::beep(uint frequency, uint duration) {
  ledcWriteTone(DEFAULT_PWM_CHANNEL, frequency);
  _isBeeping = true;
  _beepTimer.once_ms(duration, handleBeepFinishCallback, this);
}

void ESP32Buzzer::handleBeepFinishCallback(ESP32Buzzer *buzzer) {
  buzzer->silenceBuzzer();
  buzzer->_isBeeping = false;
}

void ESP32Buzzer::doubleBeep(uint frequency, uint duration,
                             uint pauseDuration) {
  _beepFrequency = frequency;
  _beepDuration = duration;
  _isDoubleBeepSecondStage = false;     // Indicates the first beep is being played
  beep(frequency, duration); // Start the first beep
  // Schedule the second beep after the first beep duration plus the pause
  // duration
  _rapidBeepTimer.once_ms(duration + pauseDuration, &handleDoubleBeepCallback, this);
}

void ESP32Buzzer::handleDoubleBeepCallback(ESP32Buzzer *buzzer) {
  if (buzzer) {
    if (!buzzer->_isDoubleBeepSecondStage) {
      // Time for the second beep
      buzzer->_isDoubleBeepSecondStage = true;
      buzzer->beep(buzzer->_beepFrequency, buzzer->_beepDuration);
    } else {
      // Second beep is done, stop beeping
      buzzer->stop();
    }
  }
}

void ESP32Buzzer::rapidBeep(uint frequency, uint duration, uint pauseInterval) {
  _rapidBeepFrequency = frequency;
  _rapidBeepDuration = duration;
  _rapidBeepPauseDuration = pauseInterval;
  _isRapidBeeping = true;

  beep(frequency, duration);
  _rapidBeepTimer.once_ms(duration + pauseInterval, &handleRapidBeepCallback, this);
}

void ESP32Buzzer::handleRapidBeepCallback(ESP32Buzzer *buzzer) {
  if (buzzer && buzzer->_isRapidBeeping) {
    buzzer->beep(buzzer->_rapidBeepFrequency, buzzer->_rapidBeepDuration);
    // Schedule the next beep after the current beep duration PLUS the pause
    // interval.
    buzzer->_rapidBeepTimer.once_ms(buzzer->_rapidBeepDuration +
                                        buzzer->_rapidBeepPauseDuration,
                                    handleRapidBeepCallback, buzzer);
  }
}

void ESP32Buzzer::stop() {
  if (_isBeeping) {
    silenceBuzzer();                   // Stop the buzzer sound instantly
    _beepTimer.detach();              // Detach the timer to stop callbacks
    _isBeeping = false;
  }
  if (_isRapidBeeping) {
    _isRapidBeeping = false;
    _rapidBeepTimer.detach();
  }
}

void ESP32Buzzer::silenceBuzzer() { ledcWriteTone(DEFAULT_PWM_CHANNEL, 0); }

#endif // PLATFORM_ESP32

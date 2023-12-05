#ifdef PLATFORM_ESP32

#include "ESP32Buzzer.h"

const int pwmChannel = 0;
const int pwmResolution = 8;
const int pwmFrequency = 2713;

ESP32Buzzer::ESP32Buzzer(const HardwarePinConfig &config)
    : BuzzerBase(config), _isBeeping(false) {
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(config.pinNumber, pwmChannel);
  setInitialized(true);
}

ESP32Buzzer::~ESP32Buzzer() {
  stop(); // Ensure the buzzer is stopped
}

void ESP32Buzzer::setVolume(float volume) {
  int dutyCycle = std::max(0.0f, std::min(volume, 1.0f)) * 255;
  ledcWrite(pwmChannel, dutyCycle);
}

void ESP32Buzzer::beep(int frequency, int duration) {
  ledcWriteTone(pwmChannel, frequency);
  _isBeeping = true;
  _timer.once_ms(duration, timerCallback, this);
}

void ESP32Buzzer::doubleBeep(int frequency, int duration, int pauseDuration) {
  _beepFrequency = frequency;
  _beepDuration = duration;
  _isSecondBeep = false; // Indicates the first beep is being played
  beep(frequency, duration);
  _timer.once_ms(duration + pauseDuration, timerCallback, this);
}

void ESP32Buzzer::timerCallback(ESP32Buzzer *buzzer) {
  if (buzzer) {
    if (!buzzer->_isSecondBeep) {
      // If first beep completed, start the second beep
      buzzer->_isSecondBeep = true;
      buzzer->beep(buzzer->_beepFrequency, buzzer->_beepDuration);
    } else {
      // If second beep completed, stop beeping
      buzzer->stop();
    }
  }
}

void ESP32Buzzer::stop() {
  if (_isBeeping) {
    ledcWriteTone(pwmChannel, 0);
    _timer.detach();
    _isBeeping = false;
  }
}

bool ESP32Buzzer::isBeeping() const { return _isBeeping; }

#endif // PLATFORM_ESP32

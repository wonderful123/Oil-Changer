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
  Logger::info("Buzzer initialized on pin: " +
               std::to_string(config.pinNumber));
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
  // Logger::info("Buzzer beeping at frequency: " + std::to_string(frequency) +
  //              " Hz for " + std::to_string(duration) + " ms");
}

void ESP32Buzzer::stop() {
  if (_isBeeping) {
    ledcWriteTone(pwmChannel, 0);
    _timer.detach();
    _isBeeping = false;
  }
}

bool ESP32Buzzer::isBeeping() const { return _isBeeping; }

void ESP32Buzzer::timerCallback(ESP32Buzzer *buzzer) {
  if (buzzer) {
    buzzer->stop();
  }
}

void ESP32Buzzer::applySettings() {
  BuzzerBase::applySettings();
  // Handle custom beep patterns
  if (currentSettings.pattern == "double-beep") {
    // Implement the double-beep pattern logic
  }
}

#endif // PLATFORM_ESP32

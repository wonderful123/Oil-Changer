#include "Buzzer.h"

#ifdef PLATFORM_ESP32

const int pwmChannel = 0;
const int pwmResolution = 8;
const int pwmFrequency = 2713;

Buzzer::Buzzer(const GpioPinConfig &config)
    : IBuzzer(config), _isBeeping(false) {
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(config.pinNumber, pwmChannel);
  setInitialized(true);
  Logger::info("Buzzer initialized on pin: " +
               std::to_string(config.pinNumber));
}

Buzzer::~Buzzer() {
  stop(); // Ensure the buzzer is stopped
}

void Buzzer::setVolume(float volume) {
  int dutyCycle = std::max(0.0f, std::min(volume, 1.0f)) * 255;
  ledcWrite(pwmChannel, dutyCycle);
  Logger::info("Buzzer volume set to: " + std::to_string(volume));
}

void Buzzer::beep(int frequency, int duration) {
  ledcWriteTone(pwmChannel, frequency);
  _timer.once_ms(duration, timerCallback, this);
  _isBeeping = true;
  // Logger::info("Buzzer beeping at frequency: " + std::to_string(frequency) +
  //              " Hz for " + std::to_string(duration) + " ms");
}

void Buzzer::stop() {
  if (_isBeeping) {
    ledcWriteTone(pwmChannel, 0);
    _timer.detach();
    _isBeeping = false;
  }
}

bool Buzzer::isBeeping() const { return _isBeeping; }

void Buzzer::timerCallback(Buzzer *buzzer) {
  if (buzzer) {
    buzzer->stop();
  }
}

#endif // PLATFORM_ESP32

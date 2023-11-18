#include "Buzzer.h"

#ifdef PLATFORM_ESP32

const int pwmChannel = 0;
const int pwmResolution = 8;
const int pwmFrequency = 5000;

Buzzer::Buzzer(const GpioPinConfig &config) : IBuzzer(config) {
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(_pinNumber, pwmChannel);
  setInitialized(true); // Mark the buzzer as initialized
}

Buzzer::~Buzzer() { stop(); }

void Buzzer::setVolume(float volume) {
  int dutyCycle = std::max(0.0f, std::min(volume, 1.0f)) * 255;
  ledcWrite(pwmChannel, dutyCycle);
}

void Buzzer::beep(int frequency, int duration) {
  ledcWriteTone(pwmChannel, frequency);
  _timer.once_ms(duration, timerCallback, this);
}

void Buzzer::stop() {
  ledcWriteTone(pwmChannel, 0);
  _timer.detach();
}

void Buzzer::update() {
  // Update logic if required
}

void Buzzer::timerCallback(Buzzer *buzzer) {
  if (buzzer) {
    buzzer->stop();
  }
}

#endif // PLATFORM_ESP32

#include "Buzzer.h"

#ifdef PLATFORM_ESP32

// Constants for LEDC
const int pwmChannel = 0;      // Use an appropriate channel
const int pwmResolution = 8;   // 8-bit resolution
const int pwmFrequency = 5000; // Adjust as needed

Buzzer::Buzzer(const GpioPinConfig &config) : _pin(config.pinNumber) {
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(_pin, pwmChannel);
}

Buzzer::~Buzzer() { stop(); }

void Buzzer::setVolume(float volume) {
  // Map volume (0.0 - 1.0) to PWM duty cycle (0 - 255 for 8-bit resolution)
  int dutyCycle = volume * 255;
  ledcWrite(pwmChannel, dutyCycle);
}

void Buzzer::beep(int frequency, int duration) {
  ledcWriteTone(pwmChannel, frequency);
  _timer.once_ms(duration, timerCallback, this);
}

void Buzzer::stop() {
  ledcWriteTone(pwmChannel, 0); // Stop the tone
  _timer.detach();              // Stop the timer
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

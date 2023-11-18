#pragma once

#include "IBuzzer.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>
#include <Ticker.h>

class Buzzer : public IBuzzer {
public:
  Buzzer(const GpioPinConfig &config) : IBuzzer(config) {
    // Initialize the buzzer hardware
    pinMode(_pinNumber, OUTPUT);
    _timer = Ticker();
    setInitialized(true); // Mark Buzzer as initialized
  }

  virtual ~Buzzer() {
    stop(); // Ensure the buzzer is stopped
  }

  void beep(int frequency, int duration) override {
    // Implementation of beep functionality
  }

  void stop() override {
    // Stop the buzzer
  }

  void update() override {
    // Update method implementation
  }

  void setVolume(float volume) override {
    // Set the volume if applicable
  }

private:
  Ticker _timer; // Ticker object for software timing

  static void timerCallback(Buzzer *buzzer) {
    // Static callback for the ticker
  }
};

#endif // PLATFORM_ESP32

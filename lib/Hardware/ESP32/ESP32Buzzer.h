#pragma once

#ifdef PLATFORM_ESP32

#include "IBuzzer.h"
#include <Arduino.h>
#include <Ticker.h>


class ESP32Buzzer : public IBuzzer {
public:
  ESP32Buzzer(const HardwarePinConfig &config);
  virtual ~ESP32Buzzer();
  void beep(int frequency, int duration) override;
  void stop() override;
  bool isBeeping() const override;
  void setVolume(float volume) override;

private:
  Ticker _timer; // Ticker object for software timing
  bool _isBeeping;

  static void timerCallback(ESP32Buzzer *buzzer);
};

#endif // PLATFORM_ESP32

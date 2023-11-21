#pragma once

#include "IBuzzer.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>
#include <Ticker.h>


class Buzzer : public IBuzzer {
public:
  Buzzer(const GpioPinConfig &config);
  virtual ~Buzzer();
  void beep(int frequency, int duration) override;
  void stop() override;
  bool isBeeping() const override;
  void setVolume(float volume) override;

private:
  Ticker _timer; // Ticker object for software timing
  bool _isBeeping;

  static void timerCallback(Buzzer *buzzer);
};

#endif // PLATFORM_ESP32

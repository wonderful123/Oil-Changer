#pragma once

#ifdef PLATFORM_ESP32

#include "BuzzerBase.h"
#include <Arduino.h>
#include <Ticker.h>

class ESP32Buzzer : public BuzzerBase {
public:
  ESP32Buzzer(const HardwarePinConfig &config);
  virtual ~ESP32Buzzer();

  void beep(int frequency, int duration) override;
  void doubleBeep(int frequency, int duration, int pauseDuration) override;
  void stop() override;
  bool isBeeping() const override;
  void setVolume(float volume) override;

private:
  Ticker _timer;
  bool _isBeeping;
  int _beepFrequency;
  int _beepDuration;
  bool _isSecondBeep;

  static void timerCallback(ESP32Buzzer *buzzer);
};

#endif // PLATFORM_ESP32

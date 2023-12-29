#pragma once

#ifdef PLATFORM_ESP32

#include "BuzzerBase.h"
#include <Arduino.h>
#include <Ticker.h>

class ESP32Buzzer : public BuzzerBase {
public:
  ESP32Buzzer(const HardwarePinConfig &config);
  virtual ~ESP32Buzzer();

  void beep(uint frequency, uint duration) override;
  void doubleBeep(uint frequency, uint duration, uint pauseDuration) override;
  static void doubleBeepCallback(ESP32Buzzer *buzzer);
  void stop() override;
  bool isBeeping() const override;
  void setVolume(float volume) override;

  void rapidBeep(uint frequency, uint duration, uint pauseInterval);
  static void rapidBeepCallback(ESP32Buzzer *buzzer);

private:
  Ticker _timer;
  Ticker _rapidBeepTimer;

  bool _isBeeping;
  bool _isRapidBeeping;

  uint _beepFrequency;
  uint _beepDuration;
  uint _rapidBeepFrequency;
  uint _rapidBeepDuration;
  uint _rapidBeepPauseInterval;

  bool _isSecondBeep;

  static void timerCallback(ESP32Buzzer *buzzer);
};

#endif // PLATFORM_ESP32

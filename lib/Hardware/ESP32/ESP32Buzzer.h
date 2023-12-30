#pragma once

#ifdef PLATFORM_ESP32

#include "BuzzerBase.h"
#include <Arduino.h>
#include <Ticker.h>

class ESP32Buzzer : public BuzzerBase {
public:
  ESP32Buzzer(const HardwarePinConfig &config);
  virtual ~ESP32Buzzer();

  void adjustVolume(float volume) override;
  void beep(uint frequency, uint duration) override;
  void doubleBeep(uint frequency, uint duration, uint pauseDuration) override;
  void rapidBeep(uint frequency, uint duration, uint pauseInterval);
  void stop() override;
  void silenceBuzzer();

private:
  Ticker _beepTimer;
  Ticker _rapidBeepTimer;

  static void handleBeepFinishCallback(ESP32Buzzer *buzzer);
  static void handleDoubleBeepTimerCallback(ESP32Buzzer *buzzer);
  static void handleRapidBeepTimerCallback(ESP32Buzzer *buzzer);
};

#endif // PLATFORM_ESP32

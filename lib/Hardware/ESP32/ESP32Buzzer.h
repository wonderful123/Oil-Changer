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
  static void handleDoubleBeepCallback(ESP32Buzzer *buzzer);
  void stop() override;
  void silenceBuzzer();
  void adjustVolume(float volume) override;

  void rapidBeep(uint frequency, uint duration, uint pauseInterval);
  static void handleRapidBeepCallback(ESP32Buzzer *buzzer);

private:
  Ticker _beepTimer;
  Ticker _rapidBeepTimer;

  static void handleBeepFinishCallback(ESP32Buzzer *buzzer);
};

#endif // PLATFORM_ESP32

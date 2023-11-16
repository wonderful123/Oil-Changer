#pragma once

#include "GpioPinConfig.h"
#include "IBuzzer.h"

#ifdef PLATFORM_ESP32
#include <Arduino.h>
#include <Ticker.h> // Include the Ticker library

class Buzzer : public IBuzzer {
public:
  /**
   * @brief Construct a new Buzzer object for ESP32.
   *
   * @param config Configuration for the buzzer pin.
   */
  Buzzer(const GpioPinConfig &config);
  virtual ~Buzzer();

  void beep(int frequency, int duration) override;
  void stop() override;
  void update() override;
  void setVolume(float volume);

private:
  int _pin;      // GPIO pin for the buzzer
  Ticker _timer; // Ticker object for software timing

  static void timerCallback(Buzzer *buzzer); // Static callback for the ticker
};
#endif // PLATFORM_ESP32
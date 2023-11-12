#pragma once

#include "ComponentConfig.h"
#include "IBuzzer.h"
#include <Arduino.h>
#include <Ticker.h> // Include the Ticker library

class Buzzer : public IBuzzer {
public:
  Buzzer(int pin, const ComponentConfig &config);
  virtual ~Buzzer();

  void beep(int frequency, int duration) override;
  void stop() override;
  void update() override;
  void setVolume(float volume);

private:
  int _pin;           // GPIO pin for the buzzer
  ConfigMap _options; // Configuration options for the buzzer
  Ticker _timer;      // Ticker object for software timing

  static void timerCallback(Buzzer *buzzer); // Static callback for the ticker
};

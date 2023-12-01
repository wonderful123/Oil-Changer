#pragma once

#include "IBuzzer.h"

class BuzzerBase : public IBuzzer {
public:
  BuzzerBase(const HardwarePinConfig &config)
      : IBuzzer(config), currentSettings({}), _volume(1.0f) {}

  virtual ~BuzzerBase() = default;

  void updateSettings(const InteractionSettings &settings) override {
    currentSettings = settings;
    // Interpret settings specific to buzzer UX
    applySettings();
  }

  void setVolume(float volume) override {
    _volume =
        std::max(0.0f, std::min(volume, 1.0f)); // Clamp volume between 0 and 1
    applyVolume();
  }

  // These methods must be implemented by derived classes
  virtual void beep(int frequency, int duration) = 0;
  virtual void stop() = 0;
  virtual bool isBeeping() const = 0;
  virtual void setVolume(float volume) = 0;

protected:
  float _volume;

  virtual void applySettings() { applyVolume(); }

  virtual void applyVolume() {
    // Implementation specific to derived class
    // For example, adjust PWM duty cycle for buzzer volume
  }
};

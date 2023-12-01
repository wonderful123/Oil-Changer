#pragma once

#include "IBuzzer.h"

class BuzzerBase : public IBuzzer {
public:
  BuzzerBase(const HardwarePinConfig &config)
      : IBuzzer(config), currentSettings({}), _volume(1.0f) {}

  virtual ~BuzzerBase() = default;

  void updateSettings(const InteractionSettings &settings) override {
    currentSettings = settings;
    applySettings();
  }

  virtual void setVolume(float volume) override {
    _volume =
        std::max(0.0f, std::min(volume, 1.0f)); // Clamp volume between 0 and 1
  }

  // These methods must be implemented by derived classes
  virtual void beep(int frequency, int duration) = 0;
  virtual void doubleBeep(int frequency, int duration, int pauseDuration) = 0;
  virtual void stop() = 0;
  virtual bool isBeeping() const = 0;

  virtual void beep() override {
    beep(currentSettings.beepSettings.standardFrequency,
         currentSettings.beepSettings.standardDurationMs);
  }

  virtual void doubleBeep() override {
    doubleBeep(currentSettings.beepSettings.limitReachedPattern.frequency,
               currentSettings.beepSettings.limitReachedPattern.durationMs,
               100); // Assuming 100ms as the pause duration
  }

protected:
  float _volume;
  InteractionSettings currentSettings;

  virtual void applySettings() {  }
};

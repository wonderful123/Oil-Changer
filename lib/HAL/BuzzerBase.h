#pragma once

#include "IBuzzer.h"
#include "Logger.h"

class BuzzerBase : public IBuzzer {
public:
  BuzzerBase(const HardwarePinConfig &config)
      : IBuzzer(config), _volume(1.0f) {}

  virtual ~BuzzerBase() = default;

  void updateSettings(std::shared_ptr<InteractionSettings> &settings) override {
    _interactionSettings = settings;
    setVolume(settings->beepSettings.buzzerVolume);
  }

  virtual void setVolume(float volume) override {
    _volume =
        std::max(0.0f, std::min(volume, 1.0f)); // Clamp volume between 0 and 1
  }

  // These methods must be implemented by derived classes
  virtual void beep(uint frequency, uint duration) = 0;
  virtual void doubleBeep(uint frequency, uint duration,
                          uint pauseDuration) = 0;
  virtual void rapidBeep(uint frequency, uint duration, uint interval) = 0;
  virtual void stop() = 0;
  virtual bool isBeeping() const = 0;

  void rapidBeep() override {
    uint defaultFrequency =
        _interactionSettings->beepSettings.rapidBeepFrequency;
    uint defaultDuration = _interactionSettings->beepSettings.rapidBeepDuration;
    uint defaultInterval =
        _interactionSettings->beepSettings.rapidBeepPauseDuration;
    rapidBeep(defaultFrequency, defaultDuration, defaultInterval);
  }

  virtual void beep() override {
    beep(_interactionSettings->beepSettings.standardFrequency,
         _interactionSettings->beepSettings.standardDurationMs);
  }

  virtual void doubleBeep() override {
    doubleBeep(
        _interactionSettings->beepSettings.limitReachedPattern.frequency,
        _interactionSettings->beepSettings.limitReachedPattern.durationMs,
        _interactionSettings->beepSettings.limitReachedPattern.pauseDuration); // Assuming 100ms as the pause duration
  }

protected:
  float _volume;
  std::shared_ptr<InteractionSettings> _interactionSettings;
};

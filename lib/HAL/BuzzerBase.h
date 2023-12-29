#pragma once

#include "IBuzzer.h"
#include "Logger.h"

class BuzzerBase : public IBuzzer {
public:
  BuzzerBase(const HardwarePinConfig &config)
      : IBuzzer(config), _volume(1.0f) {}

  virtual ~BuzzerBase() = default;

  void updateSettings(std::shared_ptr<InteractionSettings> &settings) override {
    setVolume(settings->beepSettings.buzzerVolume);

    _beepFrequency = settings->beepSettings.standardFrequency;
    _beepDuration = settings->beepSettings.standardDurationMs;
    _rapidBeepFrequency = settings->beepSettings.rapidBeepFrequency;
    _rapidBeepDuration = settings->beepSettings.rapidBeepDuration;
    _rapidBeepPauseInterval = settings->beepSettings.rapidBeepPauseDuration;
    _doubleBeepFrequency = settings->beepSettings.limitReachedPattern.frequency;
    _doubleBeepDuration = settings->beepSettings.limitReachedPattern.durationMs;
    _doubleBeepPauseDuration =
        settings->beepSettings.limitReachedPattern.pauseDuration;
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

  void rapidBeep() override {
    rapidBeep(_rapidBeepFrequency, _rapidBeepDuration, _rapidBeepPauseInterval);
  }

  virtual void beep() override { beep(_beepFrequency, _beepDuration); }

  virtual void doubleBeep() override {
    doubleBeep(_doubleBeepFrequency, _doubleBeepDuration,
               _doubleBeepPauseDuration);
  }

  bool isBeeping() override { return _isBeeping; }

protected:
  float _volume;
  bool _isBeeping;
  bool _isSecondBeep;
  bool _isRapidBeeping;

  uint _beepFrequency;
  uint _beepDuration;
  uint _rapidBeepFrequency;
  uint _rapidBeepDuration;
  uint _rapidBeepPauseInterval;
  uint _doubleBeepFrequency;
  uint _doubleBeepDuration;
  uint _doubleBeepPauseDuration;
};

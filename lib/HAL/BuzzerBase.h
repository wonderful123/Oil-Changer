#pragma once

#include "IBuzzer.h"
#include "InteractionSettings.h"

struct BeepSettings {
  uint standardFrequency;
  uint standardDurationMs;
  uint rapidBeepFrequency;
  uint rapidBeepDuration;
  uint rapidBeepPauseDuration;
  uint doubleBeepFrequency;
  uint doubleBeepDuration;
  uint doubleBeepPauseDuration;
};

struct BuzzerState {
  bool isBeeping;
  bool isDoubleBeepSecondStage;
  bool isRapidBeeping;
};

class BuzzerBase : public IBuzzer {
public:
  BuzzerBase(const HardwarePinConfig &config)
      : IBuzzer(config), _volume(1.0f) {}

  virtual ~BuzzerBase() = default;

  // These methods must be implemented by derived classes
  virtual void beep(uint frequency, uint duration) = 0;
  virtual void doubleBeep(uint frequency, uint duration,
                          uint pauseDuration) = 0;
  virtual void rapidBeep(uint frequency, uint duration, uint interval) = 0;
  virtual void stop() = 0;

  void updateSettings(std::shared_ptr<InteractionSettings> &settings) override {
    adjustVolume(settings->beepSettings.buzzerVolume);

    _settings = {settings->beepSettings.standardFrequency,
                 settings->beepSettings.standardDurationMs,
                 settings->beepSettings.rapidBeepFrequency,
                 settings->beepSettings.rapidBeepDuration,
                 settings->beepSettings.rapidBeepPauseDuration,
                 settings->beepSettings.limitReachedPattern.frequency,
                 settings->beepSettings.limitReachedPattern.durationMs,
                 settings->beepSettings.limitReachedPattern.pauseDuration};
  }

  virtual void adjustVolume(float volume) override {
    _volume =
        std::max(0.0f, std::min(volume, 1.0f)); // Clamp volume between 0 and 1
  }

  void rapidBeep() override {
    rapidBeep(_settings.rapidBeepFrequency, _settings.rapidBeepDuration,
              _settings.rapidBeepPauseDuration);
  }

  virtual void beep() override {
    beep(_settings.standardFrequency, _settings.standardDurationMs);
  }

  virtual void doubleBeep() override {
    doubleBeep(_settings.doubleBeepFrequency, _settings.doubleBeepDuration,
               _settings.doubleBeepPauseDuration);
  }

  bool isBeeping() override { return _state.isBeeping; }

  void setOnRapidBeepCallback(std::function<void()> callback) override {
    _onRapidBeepCallback = std::move(callback);
  }

protected:
  float _volume;
  BeepSettings _settings;
  BuzzerState _state;

  std::function<void()> _onRapidBeepCallback;
};

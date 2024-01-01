#pragma once

#include <string>
#include <unordered_map>

struct InteractionSettings {
  struct Acceleration {
    uint startAfterMs;
    uint rateDecreaseuintervalMs;
    uint minimumRateMs;
  };

  struct AutoRepeat {
    uint initialDelayMs;
    uint standardRateMs;
    Acceleration acceleration;
  };

  struct Button {
    std::string description;
    bool hasAutoRepeat; // Indicate if this button has auto-repeat functionality
  };

  struct LimitReachedPattern {
    std::string description;
    uint frequency;
    uint durationMs;
    uint pauseDuration;
    std::string pattern;
  };

  struct BeepSettings {
    std::string description;
    uint standardFrequency;
    uint standardDurationMs;
    float buzzerVolume;
    uint rapidBeepDuration;
    uint rapidBeepFrequency;
    uint rapidBeepPauseDuration;
    LimitReachedPattern limitReachedPattern;
  };

  struct Feedback {
    std::string description;
    bool onButtonRelease;
    bool audioFeedbackOnLimit;
  };

  struct CommonSettings {
    uint debounceMs; // Common debounce setting for all buttons
    AutoRepeat autoRepeat;
  };

  std::string description;
  CommonSettings commonSettings; // Common settings applicable to all buttons
  std::unordered_map<std::string, Button>
      buttons; // Settings specific to each button
  BeepSettings beepSettings;
  Feedback feedback;
};

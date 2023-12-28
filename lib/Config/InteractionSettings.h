#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

struct InteractionSettings {
  struct Acceleration {
    int startAfterMs;
    int rateDecreaseIntervalMs;
    int minimumRateMs;
  };

  struct AutoRepeat {
    int initialDelayMs;
    int standardRateMs;
    Acceleration acceleration;
  };

  struct Button {
    std::string description;
    bool hasAutoRepeat; // Indicate if this button has auto-repeat functionality
  };

  struct LimitReachedPattern {
    std::string description;
    int frequency;
    int durationMs;
    std::string pattern;
  };

  struct BeepSettings {
    std::string description;
    int standardFrequency;
    int standardDurationMs;
    LimitReachedPattern limitReachedPattern;
  };

  struct Feedback {
    std::string description;
    bool onButtonRelease;
    bool audioFeedbackOnLimit;
  };

  struct CommonSettings {
    int debounceMs; // Common debounce setting for all buttons
    AutoRepeat autoRepeat;
  };

  std::string description;
  CommonSettings commonSettings; // Common settings applicable to all buttons
  std::unordered_map<std::string, Button>
      buttons; // Settings specific to each button
  BeepSettings beepSettings;
  Feedback feedback;
};

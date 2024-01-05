#pragma once

#include <string>
#include <unordered_map>

struct InteractionSettings {
  struct Acceleration {
    unsigned int startAfterMs;
    unsigned int rateDecreaseIntervalMs;
    unsigned int minimumRateMs;
  };

  struct AutoRepeat {
    unsigned int initialDelayMs;
    unsigned int standardRateMs;
    Acceleration acceleration;
  };

  struct Button {
    std::string description;
    bool hasAutoRepeat; // Indicate if this button has auto-repeat functionality
  };

  struct LimitReachedPattern {
    std::string description;
    unsigned int frequency;
    unsigned int durationMs;
    unsigned int pauseDuration;
    std::string pattern;
  };

  struct BeepSettings {
    std::string description;
    unsigned int standardFrequency;
    unsigned int standardDurationMs;
    float buzzerVolume;
    unsigned int rapidBeepDuration;
    unsigned int rapidBeepFrequency;
    unsigned int rapidBeepPauseDuration;
    LimitReachedPattern limitReachedPattern;
  };

  struct Feedback {
    std::string description;
    bool onButtonRelease;
    bool audioFeedbackOnLimit;
  };

  struct CommonSettings {
    unsigned int debounceMs; // Common debounce setting for all buttons
    AutoRepeat autoRepeat;
  };

  std::string description;
  CommonSettings commonSettings; // Common settings applicable to all buttons
  std::unordered_map<std::string, Button>
      buttons; // Settings specific to each button
  BeepSettings beepSettings;
  Feedback feedback;
};

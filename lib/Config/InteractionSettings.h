#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

struct InteractionSettings {
  struct Acceleration {
    uint32_t startAfterMs;
    uint32_t rateDecreaseIntervalMs;
    uint32_t minimumRateMs;
  };

  struct AutoRepeat {
    bool enabled; // Flag to indicate if auto-repeat is enabled for a button
    uint32_t initialDelayMs;
    uint32_t standardRateMs;
    Acceleration acceleration;
  };

  struct Button {
    std::string description;
    bool hasAutoRepeat; // Indicate if this button has auto-repeat functionality
    AutoRepeat autoRepeat; // Auto-repeat settings specific to this button (if
                           // applicable)
  };

  struct LimitReachedPattern {
    std::string description;
    uint32_t frequency;
    uint32_t durationMs;
    std::string pattern;
  };

  struct BeepSettings {
    std::string description;
    uint32_t standardFrequency;
    uint32_t standardDurationMs;
    LimitReachedPattern limitReachedPattern;
  };

  struct Feedback {
    std::string description;
    bool onButtonRelease;
    bool audioFeedbackOnLimit;
  };

  struct CommonSettings {
    uint32_t debounceMs; // Common debounce setting for all buttons
  };

  std::string description;
  CommonSettings commonSettings; // Common settings applicable to all buttons
  std::unordered_map<std::string, Button>
      buttons; // Settings specific to each button
  BeepSettings beepSettings;
  Feedback feedback;
};

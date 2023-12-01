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
    uint32_t initialDelayMs;
    uint32_t standardRateMs;
    Acceleration acceleration;
    uint32_t rapidPressDebounceMs;
  };

  struct Button {
    std::string description;
    AutoRepeat autoRepeat;
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

  std::string description;
  std::unordered_map<std::string, Button> buttons;
  BeepSettings beepSettings;
  Feedback feedback;
};
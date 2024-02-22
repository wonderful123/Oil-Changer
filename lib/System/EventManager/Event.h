// Event.h
#pragma once

#include <string>
#include <unordered_map>

enum class Event { Buzzer, OilChangeTracker, Motor, Display, State, System };

enum class Parameter {
  // Buzzer Parameters
  SingleBeep,
  RapidBeep,
  DoubleBeep,
  StopBeep,

  // OilChangeTracker Parameters
  FillCapacity,
  Reset, // For starting a new fill

  // Motor Parameters
  MotorFill,
  MotorExtract,
  MotorStop, // stop motor with ramping if enabled
  MotorHalt, // stop motor right away

  // Display Parameters
  DisplayMessage,
  SecondaryDisplayMessage,

  // Current FSM states
  Initializing,
  Ready,
  ManualFill,
  ManualExtract,
  Extracting,
  InterimTask,
  Filling,
  Complete,
  Diagnostics,
};

namespace EventUtils {
// Utility function for mapping Parameter to string
inline std::string parameterToString(Parameter parameter) {
  static const std::unordered_map<Parameter, std::string> parameterStrings = {
      {Parameter::Initializing, "Initializing"},
      {Parameter::Ready, "Ready"},
      {Parameter::ManualFill, "ManualFill"},
      {Parameter::ManualExtract, "ManualExtract"},
      {Parameter::Extracting, "Extracting"},
      {Parameter::InterimTask, "InterimTask"},
      {Parameter::Filling, "Filling"},
      {Parameter::Complete, "Complete"},
      {Parameter::Diagnostics, "Diagnostics"}
      // Add other Parameter to string mappings here...
  };

  auto it = parameterStrings.find(parameter);
  if (it != parameterStrings.end()) {
    return it->second;
  } else {
    return "Unknown Parameter";
  }
}
} // namespace EventUtils
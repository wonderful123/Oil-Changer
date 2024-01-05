// EventCommand.h
#pragma once

#include <string>

enum class EventType {
  NOEVENT,
  BUZZER_BEEP,
  OIL_CHANGE_TRACKER_UPDATE,
  MANUAL_EXTRACT_START,
  MANUAL_EXTRACT_STOP,
  MANUAL_FILL_START,
  MANUAL_FILL_STOP,
  INTERACTION_SETTINGS_CHANGED,
};

class EventData {
public:
  std::string id;
  double value;

  // Default constructor
  EventData() : id(""), value(0) {}

  EventData(std::string idString, double doubleValue = 0)
      : id(std::move(idString)), value(doubleValue){};
};
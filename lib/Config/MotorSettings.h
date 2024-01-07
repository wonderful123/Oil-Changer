// MotorSettings.h
#pragma once

#include <string>

struct MotorSettings {
  std::string description;
  float maxSpeed;
  float minSpeed;
  bool rampingEnabled;
  int rampingDurationMs;
};
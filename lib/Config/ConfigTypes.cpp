// ConfigTypes.cpp

#include "ConfigTypes.h"

// Function to get the path based on the ConfigType
std::string ConfigPaths::getPathForType(ConfigType type) {
  switch (type) {
  case ConfigType::HARDWARE:
    return HardwareConfigPath;
  case ConfigType::INTERACTION_SETTINGS:
    return InteractionSettingsConfigPath;
  case ConfigType::MOTOR:
    return MotorSettingsPath;
  default:
    return "";
  }
}

// Function to get a configuration name from the ConfigType
std::string ConfigPaths::getNameForType(ConfigType type) {
  switch (type) {
  case ConfigType::HARDWARE:
    return "HardwareConfig";
  case ConfigType::INTERACTION_SETTINGS:
    return "InteractionSettings";
  case ConfigType::MOTOR:
    return "MotorSettings";
  default:
    return "Unknown";
  }
}
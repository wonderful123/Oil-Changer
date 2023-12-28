#pragma once

#include <string>

// Enum for different types of configurations
enum class ConfigType {
  HARDWARE,
  INTERACTION_SETTINGS,
};

// Namespace for configuration paths
namespace ConfigPaths {
const std::string HardwareConfigPath = "/config/hardwareConfig.json";
const std::string InteractionSettingsConfigPath =
    "/config/interactionSettings.json";

std::string getPathForType(ConfigType type);
std::string getNameForType(ConfigType type);

}  // namespace ConfigPaths

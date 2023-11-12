#pragma once
#include <string>
#include <unordered_map>

using ConfigValue = std::string;
using ConfigMap = std::unordered_map<std::string, ConfigValue>;

struct ComponentConfig {
  int gpio = -1; // Use default values that make sense for your application
  std::string type;
  ConfigMap options;

  ComponentConfig() = default; // Default constructor

  ComponentConfig(int gpio, const std::string &type, const ConfigMap &options)
      : gpio(gpio), type(type), options(options) {}
};
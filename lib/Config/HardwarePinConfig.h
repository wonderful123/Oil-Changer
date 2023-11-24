#pragma once

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct HardwarePinConfig {
  std::string id;
  std::string type;
  int pinNumber;
  std::unordered_map<std::string, int> pins;
  std::unordered_map<std::string, std::string> options; // Additional options

  // Constructor for single-pin configuration
  HardwarePinConfig(
      const std::string &id, const std::string &type, int pinNumber,
      const std::unordered_map<std::string, std::string> &options = {})
      : id(id), type(type), pinNumber(pinNumber), options(options) {}

  // Constructor for multi-pin configuration
  HardwarePinConfig(
      const std::string &id, const std::string &type,
      const std::unordered_map<std::string, int> &pins,
      const std::unordered_map<std::string, std::string> &options = {})
      : id(id), type(type), pinNumber(-1), pins(pins), options(options) {}

  template <typename T> T getOptionAs(const std::string &key) const {
    auto it = options.find(key);
    if (it != options.end()) {
      std::stringstream ss(it->second);
      T value;
      if (ss >> value) {
        return value;
      }
    }
    return T(); // Return default value if option is not found or conversion
                // fails
  }
};

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

  // Constructors
  HardwarePinConfig(
      int pinNumber, const std::string &id, const std::string &type,
      const std::unordered_map<std::string, std::string> &options = {})
      : id(id), type(type), pinNumber(pinNumber), options(options) {}

  HardwarePinConfig(
      const std::unordered_map<std::string, int> &pins, const std::string &id,
      const std::string &type,
      const std::unordered_map<std::string, std::string> &options = {})
      : id(id), type(type), pinNumber(-1), pins(pins), options(options) {}

  // Template method for getting options
  template <typename T> T getOptionAs(const std::string &key) const;
};

#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

struct GpioPinConfig {
  int pinNumber;
  std::string id;
  std::string type;
  std::unordered_map<std::string, std::string> options; // Flexible options map

  GpioPinConfig(int pin, const std::string &id, const std::string &type)
      : pinNumber(pin), id(id), type(type) {}

  template <typename T>
  T getOptionAs(const std::string &key) const {
    auto it = options.find(key);
    if (it == options.end()) {
      return -1; // Return default value if key not found
    }

    std::istringstream iss(it->second);
    T value;
    if (!(iss >> value) || iss.fail()) {
      return -1; // Return default value if conversion fails
    }
    return value;
  }
};

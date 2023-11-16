#pragma once

#include <string>
#include <unordered_map>

struct GpioPinConfig {
  int pinNumber;
  std::string id;
  std::string type;
  std::unordered_map<std::string, std::string> options; // Flexible options map

  GpioPinConfig(int pin, const std::string &id, const std::string &type)
      : pinNumber(pin), id(id), type(type) {}

  // Optional: Method to retrieve option with type casting
  template <typename T> T getOptionAs(const std::string &key) const {
    // Implement logic to convert the string to the desired type (T) and return
    // it Handle cases where the key is not found or the conversion is not
    // possible
  }
};
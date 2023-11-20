#pragma once

#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

struct GpioPinConfig {
  int pinNumber;
  std::string id;
  std::string type;
  std::unordered_map<std::string, std::string> options;

  GpioPinConfig(int pin, const std::string &id, const std::string &type)
      : pinNumber(pin), id(id), type(type) {}

  template <typename T> T getOptionAs(const std::string &key) const {
    auto it = options.find(key);
    if (it == options.end()) {
      return defaultValue<T>();
    }

    std::istringstream iss(it->second);
    T value;
    if (!(iss >> value) || iss.fail()) {
      return defaultValue<T>();
    }
    return value;
  }

private:
  template <typename T>
  static typename std::enable_if<!std::is_same<T, std::string>::value, T>::type
  defaultValue() {
    return T(); // Return default-constructed value for non-string types
  }

  // Specialization for std::string
  template <typename T>
  static typename std::enable_if<std::is_same<T, std::string>::value, T>::type
  defaultValue() {
    return ""; // Return empty string for std::string
  }
};

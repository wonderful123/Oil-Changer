#include "HardwarePinConfig.h"

// General template definition
template <typename T>
T HardwarePinConfig::getOptionAs(const std::string &key) const {
  auto it = options.find(key);
  if (it != options.end()) {
    std::istringstream ss(it->second);
    T value;
    if (ss >> value) {
      return value;
    }
  }
  return T(); // Return default value if option not found or conversion fails
}

// Specialization for bool
template <>
bool HardwarePinConfig::getOptionAs<bool>(const std::string &key) const {
  auto it = options.find(key);
  if (it != options.end()) {
    return it->second == "true";
  }
  return false;
}

// Explicit instantiation of template methods
// Necessary to avoid linker errors
template int HardwarePinConfig::getOptionAs<int>(const std::string &key) const;
template std::string
HardwarePinConfig::getOptionAs<std::string>(const std::string &key) const;
template bool
HardwarePinConfig::getOptionAs<bool>(const std::string &key) const;

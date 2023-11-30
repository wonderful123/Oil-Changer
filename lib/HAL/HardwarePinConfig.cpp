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
template int HardwarePinConfig::getOptionAs<int>(const std::string &key) const;
template std::string
HardwarePinConfig::getOptionAs<std::string>(const std::string &key) const;
template bool
HardwarePinConfig::getOptionAs<bool>(const std::string &key) const;
template double
HardwarePinConfig::getOptionAs<double>(const std::string &key) const;

// Non-member comparison operator implementation
bool operator==(const HardwarePinConfig &lhs, const HardwarePinConfig &rhs) {
  // Compare all relevant fields
  return lhs.pinNumber == rhs.pinNumber && lhs.id == rhs.id &&
         lhs.type == rhs.type && lhs.pins == rhs.pins &&
         lhs.options == rhs.options;
}

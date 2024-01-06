// IConfig.h
#pragma once

#include <string>

class Error;

// @class IConfig
// @brief An interface that defines a contract for configuration classes.

class IConfig {
public:
  virtual ~IConfig() = default;

  // Load configuration from a JSON file
  virtual Error load(const std::string &filename) = 0;

  // Save configuration to a JSON file
  virtual Error save(const std::string &filename) const = 0;
};

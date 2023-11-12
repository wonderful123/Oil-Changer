#pragma once

#include <ArduinoJson.h>
#include <string>
#include "Error.h"

class IConfig {
public:
    virtual ~IConfig() = default;

    // Load configuration from a JSON file
    virtual Error loadConfig(const std::string& filename) = 0;

    // Save configuration to a JSON file
    virtual Error saveConfig(const std::string& filename) const = 0;

    // Validate the configuration data
    virtual Error validateConfig() const = 0;
};

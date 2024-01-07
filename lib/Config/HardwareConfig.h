// HardwareConfig.h
#pragma once

#include "BaseConfig.h"
#include "DisplayConfig.h"
#include "HardwarePinConfig.h"
#include <ArduinoJson.h>
#include <string>
#include <unordered_map>
#include <vector>

class Error;
class IFileHandler;

class HardwareConfig : public BaseConfig {
public:
  explicit HardwareConfig(std::shared_ptr<IFileHandler> fileHandler,
                          const std::string &filename);

  // Method to get the hardware configurations
  const std::vector<HardwarePinConfig> &getHardwarePinConfigs() const;

  // Add a method to get display configurations
  const std::vector<DisplayConfig> &getDisplayConfigs() const;

private:
  std::vector<HardwarePinConfig> _hardwarePinConfigs;
  std::vector<DisplayConfig> _displayConfigs; // Store display configurations

  Error parseJson(const DynamicJsonDocument &doc) override;

  // Helper methods for parsing
  Error parsePinGroup(const JsonArrayConst &groupObj, bool isMultiPin);
  HardwarePinConfig parseSinglePin(const JsonObjectConst &obj,
                                   const std::string &id,
                                   const std::string &type);
  HardwarePinConfig parseMultiPin(const JsonObjectConst &obj,
                                  const std::string &id,
                                  const std::string &type);
  void parseOptions(const JsonObjectConst &obj, HardwarePinConfig &config);
  Error parseDisplays(const JsonArrayConst &displayArray); // Added method
};
#pragma once

#include "BaseConfig.h"
#include "IFileHandler.h"
#include <ArduinoJson.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "HardwarePinConfig.h"

class HardwareConfig : public BaseConfig {
public:
  explicit HardwareConfig(IFileHandler *fileHandler);

  // Method to get the hardware configurations
  const std::vector<HardwarePinConfig> &getHardwarePinConfigs() const;

private:
  std::vector<HardwarePinConfig> _hardwarePinConfigs;

  Error parseJson(const DynamicJsonDocument &doc) override;
  Error parseSinglePin(const JsonArrayConst &singlePinArray);
  Error parseMultiPin(const JsonObjectConst &multiPinObj);
  Error parseHardwarePinGroup(const JsonObjectConst &groupObj, bool isMultiPin);
  Error parseHardwarePin(const JsonObjectConst &obj, bool isMultiPin);
  std::vector<int> extractPinNumbers(const JsonObjectConst &obj);
  void handleOptionsForSinglePin(const JsonObjectConst &obj,
                                 HardwarePinConfig &config);
};

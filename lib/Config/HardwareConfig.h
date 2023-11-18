#pragma once

#include "BaseConfig.h"
#include "IFileHandler.h"
#include <ArduinoJson.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "GpioPinConfig.h"

class HardwareConfig : public BaseConfig {
public:
  explicit HardwareConfig(IFileHandler *fileHandler);
  Error save(const std::string &filename) const override;

  virtual const std::vector<GpioPinConfig> &getGpioConfigs() const;

private:
  std::vector<GpioPinConfig> gpioConfigs;

  Error parseJson(const DynamicJsonDocument &doc) override;
  Error validateADCOptions(const JsonObjectConst &obj);
  Error parseGpioPin(const JsonObjectConst &obj);
};

#pragma once

#include "BaseConfig.h"
#include "IConfig.h"
#include "IFileHandler.h"
#include <ArduinoJson.h>
#include <string>
#include <unordered_map>
#include <vector>

struct GpioPinConfig {
  int pinNumber;
  std::string id;
  std::string type;
  std::unordered_map<std::string, int>
      options; // Stores additional options like resolution, attenuation

  // Constructor for easier initialization
  GpioPinConfig(int pin, const std::string &id, const std::string &type)
      : pinNumber(pin), id(id), type(type) {}
};

class HardwareConfig : public BaseConfig {
public:
  HardwareConfig(IFileHandler *fileHandler);
  Error save(const std::string &filename) const override;

  const std::vector<GpioPinConfig> &getGpioConfigs() const;

private:
  std::vector<GpioPinConfig> gpioConfigs;

  Error parseJson(const DynamicJsonDocument &doc) override;
  Error validateADCOptions(const JsonObjectConst &obj);
  Error parseGpioPin(const JsonObjectConst &obj);
};

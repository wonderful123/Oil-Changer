#include "HardwareConfig.h"
#include <FileGuard.h>
#include "HardwarePinConfig.h"

HardwareConfig::HardwareConfig(IFileHandler *fileHandler)
    : BaseConfig(fileHandler) {
  load("/config/hardwareConfig.json");
}

const std::vector<HardwarePinConfig> &
HardwareConfig::getHardwarePinConfigs() const {
  return _hardwarePinConfigs;
}

Error HardwareConfig::parseJson(const DynamicJsonDocument &doc) {
  if (!doc.containsKey("components")) {
    return Error(Error::JsonInputInvalid);
  }

  // Parse singlePin and multiPin components
  static const std::pair<const char *, bool> componentTypes[] = {
      {"singlePin", false}, {"multiPin", true}};

  for (const auto &component : componentTypes) {
    if (doc["components"].containsKey(component.first)) {
      Error error = parseHardwarePinGroup(doc["components"][component.first],
                                          component.second);
      if (error) {
        return error;
      }
    }
  }

  return Error(Error::OK);
}

Error HardwareConfig::parseHardwarePinGroup(const JsonObjectConst &groupObj,
                                            bool isMultiPin) {
  for (auto kv : groupObj) {
    JsonObjectConst obj = kv.value().as<JsonObjectConst>();
    Error pinParseError = parseHardwarePin(obj, isMultiPin);
    if (pinParseError) {
      return pinParseError;
    }
  }
  return Error(Error::OK);
}

Error HardwareConfig::parseHardwarePin(const JsonObjectConst &obj,
                                       bool isMultiPin) {
  std::string id = obj["id"].as<std::string>();
  std::string type = obj["type"].as<std::string>();

  if (isMultiPin) {
    std::unordered_map<std::string, int> pinNumbers;

    for (auto kv : obj) {
      if (kv.key() != "id" && kv.key() != "type" && kv.key() != "options") {
        pinNumbers[kv.key().c_str()] = kv.value().as<int>();
      }
    }

    HardwarePinConfig config(pinNumbers, id, type);
    // Optionally handle options for multi-pin configurations
    _hardwarePinConfigs.push_back(config);
  } else {
    int pin = obj["pinNumber"].as<int>();
    HardwarePinConfig config(pin, id, type);
    handleOptionsForSinglePin(obj, config);
    _hardwarePinConfigs.push_back(config);
  }
  return Error(Error::OK);
}

std::vector<int> HardwareConfig::extractPinNumbers(const JsonObjectConst &obj) {
  std::vector<int> pinNumbers;
  for (auto kv : obj) {
    if (kv.key() != "id" && kv.key() != "type" && kv.key() != "options") {
      pinNumbers.push_back(kv.value().as<int>());
    }
  }
  return pinNumbers;
}

void HardwareConfig::handleOptionsForSinglePin(const JsonObjectConst &obj,
                                               HardwarePinConfig &config) {
  if (obj.containsKey("options")) {
    JsonObjectConst options = obj["options"].as<JsonObjectConst>();
    for (auto kv : options) {
      config.options[kv.key().c_str()] = kv.value().as<std::string>();
    }
  }
}

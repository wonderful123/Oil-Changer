#include "HardwareConfig.h"
#include "HardwarePinConfig.h"
const std::vector<HardwarePinConfig> &
HardwareConfig::getHardwarePinConfigs() const {
  return _hardwarePinConfigs;
}

Error HardwareConfig::parseJson(const DynamicJsonDocument &doc) {
  if (!doc.containsKey("components")) {
    return Error(Error::HardwareConfigComponentsKeyMissing);
  }

  if (doc["components"].containsKey("singlePin")) {
    Error error = parsePinGroup(
        doc["components"]["singlePin"].as<JsonArrayConst>(), false);
    if (error)
      return error;
  }

  if (doc["components"].containsKey("multiPin")) {
    Error error =
        parsePinGroup(doc["components"]["multiPin"].as<JsonArrayConst>(), true);
    if (error)
      return error;
  }

  return Error(Error::OK);
}

Error HardwareConfig::parsePinGroup(const JsonArrayConst &groupObj,
                                    bool isMultiPin) {
  for (JsonObjectConst obj : groupObj) {
    std::string id = obj["id"].as<std::string>();
    std::string type = obj["type"].as<std::string>();

    HardwarePinConfig config = isMultiPin ? parseMultiPin(obj, id, type)
                                          : parseSinglePin(obj, id, type);
    parseOptions(obj, config);
    _hardwarePinConfigs.push_back(config);
  }
  return Error(Error::OK);
}

HardwarePinConfig HardwareConfig::parseSinglePin(const JsonObjectConst &obj,
                                                 const std::string &id,
                                                 const std::string &type) {
  int pin = obj["pinNumber"].as<int>();
  return HardwarePinConfig(pin, id, type);
}

HardwarePinConfig HardwareConfig::parseMultiPin(const JsonObjectConst &obj,
                                                const std::string &id,
                                                const std::string &type) {
  JsonObjectConst pinsObj = obj["pins"].as<JsonObjectConst>();
  std::unordered_map<std::string, int> pins;
  for (auto pin : pinsObj) {
    pins[pin.key().c_str()] = pin.value().as<int>();
  }
  return HardwarePinConfig(pins, id, type);
}

void HardwareConfig::parseOptions(const JsonObjectConst &obj,
                                  HardwarePinConfig &config) {
  if (obj.containsKey("options")) {
    JsonObjectConst options = obj["options"].as<JsonObjectConst>();
    for (auto kv : options) {
      config.options[kv.key().c_str()] = kv.value().as<std::string>();
    }
  }
}

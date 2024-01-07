// HardwareConfig.cpp

#include "HardwareConfig.h"
#include "HardwarePinConfig.h"
#include "Logger.h"
#include <ArduinoJson.h>
#include <string>

HardwareConfig::HardwareConfig(std::shared_ptr<IFileHandler> fileHandler,
                               const std::string &filename)
    : BaseConfig(fileHandler, filename) {}

const std::vector<HardwarePinConfig> &
HardwareConfig::getHardwarePinConfigs() const {
  return _hardwarePinConfigs;
}

const std::vector<DisplayConfig> &HardwareConfig::getDisplayConfigs() const {
  return _displayConfigs;
}

Error HardwareConfig::parseJson(const DynamicJsonDocument &doc) {
  Logger::info("[HardwareConfig] Parsing JSON document...");

  if (!doc.containsKey("components")) {
    return Error(Error::HardwareConfigComponentsKeyMissing);
  }

  // Detailed logging for singlePin
  if (doc["components"].containsKey("singlePin")) {
    Error error = parsePinGroup(
        doc["components"]["singlePin"].as<JsonArrayConst>(), false);
    if (error) {
      return error;
    }
  }

  // Detailed logging for multiPin
  if (doc["components"].containsKey("multiPin")) {
    Error error =
        parsePinGroup(doc["components"]["multiPin"].as<JsonArrayConst>(), true);
    if (error) {
      return error;
    }
  }

  if (doc["components"].containsKey("Displays")) {
    Error error =
        parseDisplays(doc["components"]["Displays"].as<JsonArrayConst>());
    if (error) {
      return error;
    }
  }

  Logger::info("[HardwareConfig] JSON document parsed successfully");
  return Error(Error::OK);
}

Error HardwareConfig::parsePinGroup(const JsonArrayConst &groupObj,
                                    bool isMultiPin) {
  for (JsonObjectConst obj : groupObj) {
    std::string id = obj["id"].as<std::string>();
    std::string type = obj["type"].as<std::string>();
    HardwarePinConfig config = isMultiPin ? parseMultiPin(obj, id, type)
                                          : parseSinglePin(obj, id, type);
    parseOptions(obj, config);             // Parse options if any
    _hardwarePinConfigs.push_back(config); // Add to list of configs
  }

  return Error(Error::OK);
}

HardwarePinConfig HardwareConfig::parseSinglePin(const JsonObjectConst &obj,
                                                 const std::string &id,
                                                 const std::string &type) {
  int pin = obj["pinNumber"].as<int>();
  Logger::info("[HardwareConfig] Parsed: " + type +
               " (pin: " + std::to_string(pin) + ", id: " + id + ")");

  return HardwarePinConfig(pin, id, type);
}

HardwarePinConfig HardwareConfig::parseMultiPin(const JsonObjectConst &obj,
                                                const std::string &id,
                                                const std::string &type) {
  JsonObjectConst pinsObj = obj["pins"];
  std::unordered_map<std::string, int> pins;

  std::string pinsLogString = "";
  for (auto pin = pinsObj.begin(); pin != pinsObj.end(); ++pin) {
    pins[pin->key().c_str()] = pin->value().as<int>();
    pinsLogString.append(std::string(pin->key().c_str()) + ": " +
                         pin->value().as<std::string>() + ", ");
  }

  // Remove trailing comma and space
  pinsLogString = pinsLogString.substr(0, pinsLogString.length() - 2);

  Logger::info("[HardwareConfig] Parsed: " + type + " (id: " + id + ", " +
               pinsLogString + ")");

  return HardwarePinConfig(pins, id, type);
}

Error HardwareConfig::parseDisplays(const JsonArrayConst &displayArray) {
  for (JsonObjectConst obj : displayArray) {
    std::string id = obj["id"].as<std::string>();
    std::string interfaceId = obj["interfaceId"].as<std::string>();

    Logger::info("[HardwareConfig] Parsed: Display (" + id +
                 " with Interface: " + interfaceId + ")");

    _displayConfigs.push_back(DisplayConfig(id, interfaceId));
  }

  return Error(Error::OK);
}

void HardwareConfig::parseOptions(const JsonObjectConst &obj,
                                  HardwarePinConfig &config) {
  // Parses the options if any and adds them to the config object passed in as a
  // parameter.
  if (obj.containsKey("options")) {
    JsonObjectConst options = obj["options"].as<JsonObjectConst>();
    for (auto kv : options) {
      config.options[kv.key().c_str()] = kv.value().as<std::string>();
    }
  }
}

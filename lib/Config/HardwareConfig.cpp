#include "HardwareConfig.h"
#include <FileGuard.h>

HardwareConfig::HardwareConfig(IFileHandler *fileHandler)
    : BaseConfig(fileHandler) {} // Initializing base class constructor

const std::vector<GpioPinConfig> &HardwareConfig::getGpioConfigs() const {
  return gpioConfigs;
}

Error HardwareConfig::parseJson(const DynamicJsonDocument &doc) {
  if (!doc.containsKey("gpioPins")) {
    return Error::HardwareConfigGpioPinsKeyMissing;
  }

  if (!doc["gpioPins"].is<JsonArrayConst>()) {
    return Error::HardwareConfigGpioPinsTypeMismatch;
  }

  JsonArrayConst gpioPins = doc["gpioPins"].as<JsonArrayConst>();
  for (JsonObjectConst obj : gpioPins) {
    Error pinParseError = parseGpioPin(obj);
    if (pinParseError) {
      return pinParseError;
    }
  }

  return Error::OK;
}

Error HardwareConfig::parseGpioPin(const JsonObjectConst &obj) {
  if (!obj.containsKey("pinNumber") || !obj["pinNumber"].is<int>())
    return Error::HardwareConfigGpioPinNumberMissing;
  int pin = obj["pinNumber"].as<int>();

  if (!obj.containsKey("id") || !obj["id"].is<std::string>())
    return Error::HardwareConfigGpioPinIdMissing;
  std::string id = obj["id"].as<std::string>();

  if (!obj.containsKey("type") || !obj["type"].is<std::string>())
    return Error::HardwareConfigGpioPinTypeMissing;
  std::string type = obj["type"].as<std::string>();

  GpioPinConfig config(pin, id, type);

  if (type == "ADC") {
    Error validationError = validateADCOptions(obj);
    if (validationError) {
      return validationError;
    }
  }

  if (obj.containsKey("options")) {
    JsonObjectConst options = obj["options"].as<JsonObjectConst>();
    for (auto kv : options) {
      config.options[kv.key().c_str()] = kv.value().as<int>();
    }
  }

  gpioConfigs.push_back(config);
  return Error::OK;
}

Error HardwareConfig::validateADCOptions(const JsonObjectConst &obj) {
  if (!obj.containsKey("options") || !obj["options"].is<JsonObjectConst>()) {
    return Error::HardwareConfigAdcOptionsKeyMissing;
  }

  JsonObjectConst options = obj["options"].as<JsonObjectConst>();
  if (!options.containsKey("resolution") || !options["resolution"].is<int>()) {
    return Error(Error::HardwareConfigAdcResolutionOptionMissing);
  }

  return Error::OK;
}

Error HardwareConfig::save(const std::string &filename) const {
  // Calculate the required size for the JSON document
  const size_t capacity =
      JSON_ARRAY_SIZE(gpioConfigs.size()) +
      gpioConfigs.size() *
          JSON_OBJECT_SIZE(3); // Adjust size based on your structure
  DynamicJsonDocument doc(capacity);

  // Create a JSON array
  JsonArray gpioPinsArray = doc.createNestedArray("gpioPins");

  // Serialize gpioConfigs to JSON
  for (const auto &config : gpioConfigs) {
    JsonObject pinObj = gpioPinsArray.createNestedObject();
    pinObj["pinNumber"] = config.pinNumber;
    pinObj["id"] = config.id;
    pinObj["type"] = config.type;

    // Additional fields and checks based on your GpioPinConfig structure
    if (config.type == "ADC") {
      JsonObject options = pinObj.createNestedObject("options");
      // Populate options based on your ADC config structure
    }
  }

  return writeJsonToFile(doc, filename);
}
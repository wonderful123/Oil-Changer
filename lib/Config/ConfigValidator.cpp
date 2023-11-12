#include "ConfigValidator.h"
#include "Error.h"       // Include Error
#include "Logger.h"      // Include Logger
#include <ArduinoJson.h> // Needed for JSON handling

namespace ConfigValidator {

Error validateGpioADC(const JsonObjectConst &adcConfig) {
  Logger::debug("Validating ADC GPIO Pin");

  if (!adcConfig.containsKey("options")) {
    Logger::error("Error: ADC options key missing");
    return Error::ConfigMissingADCOptionsKey;
  }

  const JsonObjectConst &options = adcConfig["options"].as<JsonObjectConst>();
  if (!options.containsKey("resolution")) {
    Logger::error("Error: ADC resolution option missing");
    return Error::ConfigMissingADCResolutionOption;
  }

  Logger::info("ADC GPIO Pin validation successful");
  return Error::OK;
}

Error validateGpioPin(const JsonObjectConst &pinConfig) {
  String serializedJson;
  serializeJson(pinConfig, serializedJson);
  Logger::debug(("Validating GPIO Pin: " + serializedJson).c_str());

  if (!pinConfig.containsKey("gpio")) {
    Logger::error(("Error: GPIO key undefined in " + serializedJson).c_str());
    return Error(Error::ConfigUndefinedGpioKeyForPin);
  }
  if (!pinConfig.containsKey("id")) {
    Logger::error(("Error: GPIO ID key missing in " + serializedJson).c_str());
    return Error(Error::ConfigMissingGpioPinId);
  }
  if (!pinConfig.containsKey("type")) {
    Logger::error(
        ("Error: GPIO type key missing in " + serializedJson).c_str());
    return Error(Error::ConfigMissingGpioPinType);
  }

  if (pinConfig["type"] == "ADC") {
    return validateGpioADC(pinConfig);
  }

  Logger::info(("GPIO Pin validation successful for ID: " +
                String(pinConfig["id"].as<String>()))
                   .c_str());
  return Error::OK;
}

Error validateConfiguration(
    const ArduinoJson::StaticJsonDocument<ConfigConstants::JSON_DOC_SIZE>
        &doc) {
  Logger::info("Starting configuration validation");

  if (!doc.containsKey("hardware")) {
    Logger::error("Error: Hardware key undefined");
    return Error(Error::ConfigUndefinedHardwareKey);
  }
  if (!doc.containsKey("gpioPins")) {
    Logger::error("Error: GPIO Pins array key missing");
    return Error(Error::ConfigUndefinedGpioPinsArrayKey);
  }

  const JsonArrayConst &gpioPinsArray = doc["gpioPins"].as<JsonArrayConst>();
  for (const JsonObjectConst &pinConfig : gpioPinsArray) {
    Error error = validateGpioPin(pinConfig);
    if (error != Error::OK) {
      Logger::error((String("Error in validating GPIO Pin: ") +
                     error.getFormattedMessage(error.code()).c_str())
                        .c_str());
      return error;
    }
  }

  Logger::info("Configuration validation successful");
  return Error::OK;
}

} // namespace ConfigValidator

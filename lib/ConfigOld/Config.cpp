#include "Config.h"
#include "ConfigValidator.h"
#include "JsonDeserializer.h"
#include <FileGuard.h>

Error Config::load(const std::string &path) {
  FileGuard fileGuard(_fileHandler, path);
  if (!fileGuard.isOpen()) {
    return Error::FileNotOpen;
  }

  std::string fileContent = fileGuard.read();
  if (fileContent.empty()) {
    return Error::FileReadError;
  }

  DeserializationError jsonError = deserializeJson(_doc, fileContent);
  if (jsonError) {
    return Error::JsonDeserializationError;
  }

  Error validationError = ConfigValidator::validateConfiguration(_doc);
  if (validationError != Error::OK) {
    return validationError;
  }

  Error populateError = populateComponentConfigs();
  return populateError; // Return any errors from populating the configuration
}

Error Config::populateComponentConfigs() {
  JsonArray gpioPins = _doc["gpioPins"].as<JsonArray>();

  for (JsonObject pinConfig : gpioPins) {
    ComponentConfig componentConfig;
    componentConfig.gpio = pinConfig["gpio"].as<int>();
    componentConfig.type = pinConfig["type"].as<std::string>();

    JsonObject options = pinConfig["options"].as<JsonObject>();
    for (JsonPair kv : options) {
      std::string value;

      if (kv.value().is<int>()) {
        value = std::to_string(kv.value().as<int>());
      } else if (kv.value().is<double>()) {
        value = std::to_string(kv.value().as<double>());
      } else if (kv.value().is<bool>()) {
        value = kv.value().as<bool>() ? "true" : "false";
      } else if (kv.value().is<const char *>()) {
        value = kv.value().as<std::string>();
      } else {
        return Error(Error::ConfigInvalidOptionType);
      }

      componentConfig.options[kv.key().c_str()] = std::move(value);
    }

    auto result = _componentsConfig.emplace(pinConfig["id"].as<std::string>(),
                                            std::move(componentConfig));
    if (!result.second) {
      // The emplace failed because the key already exists
      return Error::DuplicateComponentId;
    }
  }
  return Error::OK;
}

ComponentConfig Config::getConfigFor(const std::string &componentId) const {
  auto it = _componentsConfig.find(componentId);
  if (it != _componentsConfig.end()) {
    return it->second;
  }

  // Trigger the error logging
  Error error(Error::ConfigRequestConfigForInvalidComponent);

  // Return a dummy ComponentConfig object
  return ComponentConfig();
}

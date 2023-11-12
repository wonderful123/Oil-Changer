#pragma once

#define ARDUINOJSON_ENABLE_COMMENTS 1

#include "ComponentConfig.h"
#include "ConfigConstants.h"
#include "Error.h"
#include <ArduinoJson.h>
#include <IFileHandler.h>
#include <string>

class Config {
public:
  explicit Config(IFileHandler *fileHandler) : _fileHandler(fileHandler) {}

  Error load(const std::string &path);

  ComponentConfig getConfigFor(const std::string &componentId) const;

private:
  ArduinoJson::StaticJsonDocument<ConfigConstants::JSON_DOC_SIZE> _doc;
  std::unordered_map<std::string, ComponentConfig>
      _componentsConfig; // Keyed by component ID
  IFileHandler *_fileHandler;

  Error populateComponentConfigs();
};
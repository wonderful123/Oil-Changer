#pragma once

#include "ConfigConstants.h"
#include "Error.h"
#include <ArduinoJson.h>

class JsonDeserializer {
public:
  static ArduinoJson::StaticJsonDocument<ConfigConstants::JSON_DOC_SIZE>
  deserialize(const std::string &content, Error &error);
};

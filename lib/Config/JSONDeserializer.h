#pragma once

#include "Error.h"
#include <ArduinoJson.h>
#include <string>

class JsonDeserializer {
public:
  static DynamicJsonDocument deserialize(const std::string &content,
                                         Error &error);

private:
  static size_t estimateJsonDocumentSize(const std::string &content);
};

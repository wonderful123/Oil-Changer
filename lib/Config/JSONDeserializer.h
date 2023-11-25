#pragma once

#ifndef ARDUINOJSON_ENABLE_COMMENTS
#define ARDUINOJSON_ENABLE_COMMENTS 1
#endif

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

#include "JsonDeserializer.h"

ArduinoJson::StaticJsonDocument<ConfigConstants::JSON_DOC_SIZE>
JsonDeserializer::deserialize(const std::string &content, Error &error) {
  ArduinoJson::StaticJsonDocument<ConfigConstants::JSON_DOC_SIZE> doc;
  auto deserializationError = ArduinoJson::deserializeJson(doc, content);

  if (deserializationError) {
    // Map ArduinoJson's DeserializationError to your Error enum
    switch (deserializationError.code()) {
    case ArduinoJson::DeserializationError::InvalidInput:
      error = Error(Error::JsonInvalidInput);
      break;
    case ArduinoJson::DeserializationError::NoMemory:
      error = Error(Error::JsonNoMemory);
      break;
    case ArduinoJson::DeserializationError::EmptyInput:
      error = Error(Error::JsonEmptyInput);
      break;
    case ArduinoJson::DeserializationError::IncompleteInput:
      error = Error(Error::JsonIncompleteInput);
      break;
    default:
      error = Error(Error::JsonDeserializationError);
      break;
    }
    return {}; // Return an empty document to signal a failure
  }
  return doc; // Return the populated document on success
}

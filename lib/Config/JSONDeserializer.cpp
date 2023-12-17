#include "JSONDeserializer.h"
#include "Error.h"

DynamicJsonDocument JsonDeserializer::deserialize(const std::string &content,
                                                  Error &error) {
  size_t estimatedSize = estimateJsonDocumentSize(content);
  DynamicJsonDocument doc(estimatedSize);

  auto deserializationError = ArduinoJson::deserializeJson(doc, content);

  if (deserializationError) {
    // Map ArduinoJson's DeserializationError to your Error enum
    switch (deserializationError.code()) {
    case ArduinoJson::DeserializationError::InvalidInput:
      error = Error(Error::JsonInputInvalid);
      break;
    case ArduinoJson::DeserializationError::NoMemory:
      error = Error(Error::JsonMemoryError);
      break;
    case ArduinoJson::DeserializationError::EmptyInput:
      error = Error(Error::JsonInputEmpty);
      break;
    case ArduinoJson::DeserializationError::IncompleteInput:
      error = Error(Error::JsonInputIncomplete);
      break;
    default:
      error = Error(Error::JsonDeserializationFailure);
      break;
    }
    // Return an empty document by constructing a new one with minimal size
    return DynamicJsonDocument(0);
  }
  return doc; // Return the populated document on success
}

size_t JsonDeserializer::estimateJsonDocumentSize(const std::string &content) {
  // Simple heuristic: size of content + some extra space
  return content.length() + 4000; // Adjust based on your needs
}
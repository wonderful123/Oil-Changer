// BaseConfig.cpp
#include "BaseConfig.h"
#include "Error.h"
#include "FileGuard.h"
#include "JSONDeserializer.h"

BaseConfig::BaseConfig(std::shared_ptr<IFileHandler> fileHandler,
                       const std::string &filename)
    : _fileHandler(fileHandler), _filename(filename) {
  if (!fileHandler) {
    Error(Error::FileHandlerIsNull);
  }
}

Error BaseConfig::load() {
  FileGuard fileGuard(_fileHandler.get(), _filename, "r");
  if (!fileGuard.isOpen()) {
    return Error(Error::FileOpenFailure);
  }

  std::string fileContent = fileGuard.read();

  // Deserialize the JSON file and check for basic errors
  Error jsonError;
  DynamicJsonDocument doc =
      JsonDeserializer::deserialize(fileContent, jsonError);

  if (jsonError) {
    return jsonError;
  }

  return parseJson(doc);
}

Error BaseConfig::writeJsonToFile(const DynamicJsonDocument &doc,
                                  const std::string &filenameParameter) const {
  if (!_fileHandler) {
    return Error(Error::FileHandlerIsNull);
  }

  constexpr size_t MaxStringSize = 4000; // Maximum string size for ESP32

  if (measureJsonPretty(doc) > MaxStringSize) {
    return Error(Error::JsonDocSizeExceededMaximum4000BytesForESP32);
  }

  // Determine the filename to use. The class _filename from the constructor
  // will be used otherwise
  std::string filename =
      filenameParameter.empty() ? _filename : filenameParameter;

  // Serialize the JSON document to a string
  std::string output;
  serializeJsonPretty(doc, output);

  // Create a FileGuard for the filename in write mode
  FileGuard fileGuard(_fileHandler.get(), filename, "w");

  if (!fileGuard.isOpen()) {
    return Error(Error::FileWriteFailure);
  }

  if (!_fileHandler->write(output)) {
    // FileGuard will automatically close the file when it goes out of scope
    return Error(Error::FileWriteFailure);
  }

  Logger::info("Successfully saved: " + filename);

  // FileGuard will automatically close the file when it goes out of scope
  return Error(Error::OK);
}
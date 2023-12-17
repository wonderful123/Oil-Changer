#include "BaseConfig.h"
#include "FileGuard.h"
#include "JSONDeserializer.h"

BaseConfig::BaseConfig(std::shared_ptr<IFileHandler> fileHandler)
    : _fileHandler(std::move(fileHandler)) {}

Error BaseConfig::load(const std::string &filename) {
  FileGuard fileGuard(_fileHandler.get(), filename);
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
                                  const std::string &filename) const {
  std::string output;
  serializeJson(doc, output);

  if (!_fileHandler->open(filename, "w")) {
    return Error(Error::FileWriteFailure);
  }

  if (!_fileHandler->write(output)) {
    _fileHandler->close();
    return Error(Error::FileWriteFailure);
  }

  _fileHandler->close();
  return Error(Error::OK);
}
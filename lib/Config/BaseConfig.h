#pragma once

#include "FileGuard.h"
#include "IConfig.h"
#include "IFileHandler.h"
#include <ArduinoJson.h>
#include <string>

// @class BaseConfig
// @brief An abstract base class that provides common functionality for
// configuration classes.

class BaseConfig : public IConfig {
public:
  explicit BaseConfig(IFileHandler *fileHandler) : _fileHandler(fileHandler) {}

  virtual ~BaseConfig() = default;

  Error load(const std::string &filename) override;
  virtual Error save(const std::string &filename) const override = 0;

protected:
  virtual Error parseJson(const DynamicJsonDocument &doc) = 0;
  Error writeJsonToFile(const DynamicJsonDocument &doc,
                        const std::string &filename) const;

      IFileHandler *_fileHandler;
};

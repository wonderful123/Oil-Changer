// BaseConfig.h
#pragma once

#include "Error.h"
#include "IConfig.h"
#include <ArduinoJson.h>
#include <memory>
#include <string>

class IFileHandler;

// @class BaseConfig
// @brief An abstract base class that provides common functionality for
// configuration classes.

class BaseConfig : public IConfig {
public:
  explicit BaseConfig(std::shared_ptr<IFileHandler> fileHandler,
                      const std::string &filename);

  virtual ~BaseConfig() = default;

  virtual Error load() override;
  virtual Error save() const override { return Error(Error::OK); };

protected:
  virtual Error parseJson(const DynamicJsonDocument &doc) = 0;
  // filenameParameter is optional. If none given then the filename set in the
  // constructor is used.
  Error writeJsonToFile(const DynamicJsonDocument &doc,
                        const std::string &filenameParameter = "") const;

  std::shared_ptr<IFileHandler> _fileHandler;
  std::string _filename;
};

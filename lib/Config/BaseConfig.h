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
  explicit BaseConfig(std::shared_ptr<IFileHandler> fileHandler);

  virtual ~BaseConfig() = default;

  Error load(const std::string &filename) override;
  virtual Error save(const std::string &filename) const override {
    return Error(Error::OK);
  };

protected:
  virtual Error parseJson(const DynamicJsonDocument &doc) = 0;
  Error writeJsonToFile(const DynamicJsonDocument &doc,
                        const std::string &filename) const;

  std::shared_ptr<IFileHandler> _fileHandler;
};

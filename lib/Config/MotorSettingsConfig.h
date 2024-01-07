// MotorSettingsParser.h
#pragma once

#include "BaseConfig.h"

class Error;
class MotorSettings;

class MotorSettingsConfig : public BaseConfig {
public:
  explicit MotorSettingsConfig(std::shared_ptr<IFileHandler> fileHandler,
                               const std::string &filename);
  ~MotorSettingsConfig() override = default;

  std::shared_ptr<MotorSettings> getSettings();

  Error save() const override;

private:
  std::shared_ptr<MotorSettings> _settings;
  Error parseJson(const DynamicJsonDocument &doc) override;
  Error motorSettingsValidation(const MotorSettings &settings) const;
};
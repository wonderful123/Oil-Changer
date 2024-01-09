// MotorSettingsConfig.h
#pragma once

#include "BaseConfig.h"
#include "MotorSettings.h"

class Error;

class MotorSettingsConfig : public BaseConfig {
public:
  MotorSettingsConfig(std::shared_ptr<IFileHandler> fileHandler,
                      const std::string &filename);
  ~MotorSettingsConfig() override = default;

  MotorSettings getSettings();

  Error save() const override;

private:
  MotorSettings _settings;
  Error parseJson(const DynamicJsonDocument &doc) override;
  Error motorSettingsValidation(const MotorSettings &settings) const;
};
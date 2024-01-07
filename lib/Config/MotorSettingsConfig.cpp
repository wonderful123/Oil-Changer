// MotorSettingsConfig.cpp

#include "MotorSettingsConfig.h"
#include "Error.h"
#include "MotorSettings.h"

MotorSettingsConfig::MotorSettingsConfig(
    std::shared_ptr<IFileHandler> fileHandler, const std::string &filename)
    : BaseConfig(fileHandler, filename),
      _settings(std::make_shared<MotorSettings>()){};

Error MotorSettingsConfig::parseJson(const DynamicJsonDocument &doc) {
  const JsonObjectConst motorSettings = doc["motorSettings"];

  if (motorSettings == nullptr) {
    return Error(Error::MotorSettingsJsonMissingMotorSettingsKey);
  }

  MotorSettings settings;
  settings.description = motorSettings["description"].as<std::string>();
  settings.maxSpeed = motorSettings["maxSpeed"].as<float>();
  settings.minSpeed = motorSettings["minSpeed"].as<float>();
  settings.rampingEnabled = motorSettings["rampingEnabled"].as<bool>();
  settings.rampingOnDurationMs = motorSettings["rampingOnDurationMs"].as<int>();
  settings.rampingOffDurationMs = motorSettings["rampingOffDurationMs"].as<int>();

  Error err = motorSettingsValidation(settings);

  if (err) {
    return err;
  }

  _settings = std::make_shared<MotorSettings>(settings);
  return Error::OK;
}

Error MotorSettingsConfig::motorSettingsValidation(
    const MotorSettings &settings) const {
  // Conditions:
  // minSpeed < maxSpeed;
  // minSpeed > 0
  // rampingDurationMs >= 0
  if (settings.maxSpeed < settings.minSpeed) {
    return Error(Error::MotorSettingsValidationMaxSpeedLessThanMinSpeed);
  }

  if (settings.minSpeed <= 0) {
    return Error(Error::MotorSettingsValidationMinSpeedLessThanOrEqualToZero);
  }

  if (settings.rampingOnDurationMs < 0 || settings.rampingOffDurationMs < 0) {
    return Error(Error::MotorSettingsValidationRampingDurationLessThanZero);
  }

  return Error::OK;
}

Error MotorSettingsConfig::save() const {
  // Check if the settings are initialized
  if (!_settings) {
    return Error(Error::MotorSettingsNotInitialized);
  }

  // Create a JSON object and populate it with motor settings
  DynamicJsonDocument doc(1024);
  JsonObject jsonSettings = doc.createNestedObject("motorSettings");

  jsonSettings["description"] = _settings->description;
  jsonSettings["maxSpeed"] = _settings->maxSpeed;
  jsonSettings["minSpeed"] = _settings->minSpeed;
  jsonSettings["rampingEnabled"] = _settings->rampingEnabled;
  jsonSettings["rampingOnDurationMs"] = _settings->rampingOnDurationMs;
  jsonSettings["rampingOffDurationMs"] = _settings->rampingOffDurationMs;

  return writeJsonToFile(jsonSettings);
}

std::shared_ptr<MotorSettings> MotorSettingsConfig::getSettings() {
  return _settings;
}
// InteractionSettingsConfig.h
#pragma once

#include "BaseConfig.h"

class InteractionSettings;
class Error;

class InteractionSettingsConfig : public BaseConfig {
public:
  explicit InteractionSettingsConfig(std::shared_ptr<IFileHandler> fileHandler);

  Error save(const std::string &filename) const override;

  std::shared_ptr<InteractionSettings> getSettings();

protected:
  Error parseJson(const DynamicJsonDocument &doc) override;

  Error parseCommonSettings(const JsonObjectConst commonSettings);

  Error parseButtonSettings(const JsonObjectConst buttonObj);

  Error parseBeepSettings(const JsonObjectConst beepSettings);

  Error parseFeedbackSettings(const JsonObjectConst feedbackSettings);

private:
  std::shared_ptr<InteractionSettings> _interactionSettings;
};
#pragma once

#include "BaseConfig.h"
#include "InteractionSettings.h"

class Error;

class InteractionSettingsConfig : public BaseConfig {
public:
  explicit InteractionSettingsConfig(std::shared_ptr<IFileHandler> fileHandler);

  Error save(const std::string &filename) const override;

  InteractionSettings getSettings();

protected:
  Error parseJson(const DynamicJsonDocument &doc) override;

  Error parseButtonSettings(const JsonObjectConst &buttonObj);

  Error parseBeepSettings(const JsonObjectConst &beepSettings);

  Error parseFeedbackSettings(const JsonObjectConst &feedbackSettings);

private:
  InteractionSettings _interactionSettings;
};
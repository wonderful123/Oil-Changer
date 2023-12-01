#pragma once

#include "BaseConfig.h"
#include "Error.h"
#include "InteractionSettings.h"

class InteractionSettingsConfig : public BaseConfig {
public:
  using BaseConfig::BaseConfig;

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
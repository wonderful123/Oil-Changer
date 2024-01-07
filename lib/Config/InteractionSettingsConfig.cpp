// InteractionSettingsConfig.cpp

#include "InteractionSettingsConfig.h"
#include "Error.h"
#include "InteractionSettings.h"

InteractionSettingsConfig::InteractionSettingsConfig(
    std::shared_ptr<IFileHandler> fileHandler, const std::string &filename)
    : BaseConfig(fileHandler, filename), _filename(filename),
      _interactionSettings(std::make_shared<InteractionSettings>()) {}

Error InteractionSettingsConfig::save() const {
  // TODO: Implement this function
  return Error(Error::OK);
}

Error InteractionSettingsConfig::parseJson(const DynamicJsonDocument &doc) {
  const JsonObjectConst buttonInteraction = doc["buttonInteraction"];

  if (buttonInteraction == nullptr) {
    return Error(Error::InteractionSettingsButtonInteractionKeyMissing);
  }

  Error err = parseCommonSettings(buttonInteraction["commonSettings"]);
  if (err != Error::OK) {
    return err;
  }

  err = parseButtonSettings(buttonInteraction["buttons"]);
  if (err != Error::OK) {
    return err;
  }

  err = parseBeepSettings(buttonInteraction["beepSettings"]);
  if (err != Error::OK) {
    return err;
  }

  err = parseFeedbackSettings(buttonInteraction["feedback"]);
  if (err != Error::OK) {
    return err;
  }

  return Error(Error::OK);
}

Error InteractionSettingsConfig::parseCommonSettings(
    const JsonObjectConst commonSettings) {
  if (commonSettings == nullptr) {
    return Error(Error::InteractionSettingsCommonSettingsKeyMissing);
  }

  InteractionSettings::CommonSettings settings;

  // Parsing 'debounceMs'
  settings.debounceMs = commonSettings["debounceMs"];

  // Parsing 'autoRepeat'
  const JsonObjectConst autoRepeatObj = commonSettings["autoRepeat"];
  settings.autoRepeat.initialDelayMs = autoRepeatObj["initialDelayMs"];
  settings.autoRepeat.standardRateMs = autoRepeatObj["standardRateMs"];

  // Parsing 'acceleration'
  const JsonObjectConst accelerationObj = autoRepeatObj["acceleration"];
  settings.autoRepeat.acceleration.startAfterMs =
      accelerationObj["startAfterMs"];
  settings.autoRepeat.acceleration.rateDecreaseIntervalMs =
      accelerationObj["rateDecreaseuintervalMs"];
  settings.autoRepeat.acceleration.minimumRateMs =
      accelerationObj["minimumRateMs"];

  _interactionSettings->commonSettings = settings;

  return Error::OK;
}

Error InteractionSettingsConfig::parseButtonSettings(
    const JsonObjectConst buttonsJson) {
  if (buttonsJson == nullptr) {
    return Error::InteractionSettingsButtonsSubkeyMissing;
  }

  for (auto buttonObj : buttonsJson) {
    const std::string buttonKey = buttonObj.key().c_str();
    const JsonObjectConst buttonSettingsObj = buttonObj.value();

    InteractionSettings::Button button;
    button.description = buttonSettingsObj["description"].as<std::string>();

    // Parsing optional 'autoRepeat' flag
    if (buttonSettingsObj["autoRepeat"].as<bool>()) {
      button.hasAutoRepeat = true;
    } else {
      button.hasAutoRepeat = false;
    }

    _interactionSettings->buttons[buttonKey] = button;
  }

  return Error(Error::OK);
}

Error InteractionSettingsConfig::parseBeepSettings(
    const JsonObjectConst beepSettings) {
  if (beepSettings == nullptr) {
    return Error::InteractionSettingsBeepSettingsSubkeyMissing;
  }

  InteractionSettings::BeepSettings settings;
  settings.description = beepSettings["description"].as<std::string>();
  settings.standardFrequency =
      beepSettings["standardFrequency"].as<unsigned int>();
  settings.standardDurationMs =
      beepSettings["standardDurationMs"].as<unsigned int>();
  settings.buzzerVolume = beepSettings["buzzerVolume"].as<float>();
  settings.rapidBeepDuration =
      beepSettings["rapidBeepDuration"].as<unsigned int>();
  settings.rapidBeepFrequency =
      beepSettings["rapidBeepFrequency"].as<unsigned int>();
  settings.rapidBeepPauseDuration =
      beepSettings["rapidBeepPauseDuration"].as<unsigned int>();

  const JsonObjectConst limitPattern = beepSettings["limitReachedPattern"];
  settings.limitReachedPattern.description =
      limitPattern["description"].as<std::string>();
  settings.limitReachedPattern.frequency = limitPattern["frequency"];
  settings.limitReachedPattern.durationMs = limitPattern["durationMs"];
  settings.limitReachedPattern.pauseDuration = limitPattern["pauseDuration"];
  settings.limitReachedPattern.pattern =
      limitPattern["pattern"].as<std::string>();

  _interactionSettings->beepSettings = settings;

  return Error(Error::OK);
}

Error InteractionSettingsConfig::parseFeedbackSettings(
    const JsonObjectConst feedbackSettings) {
  if (feedbackSettings == nullptr) {
    return Error::Error::InteractionSettingsFeedbackSubkeyMissing;
  }

  InteractionSettings::Feedback feedback;
  feedback.description = feedbackSettings["description"].as<std::string>();
  feedback.onButtonRelease = feedbackSettings["onButtonRelease"];
  feedback.audioFeedbackOnLimit = feedbackSettings["audioFeedbackOnLimit"];

  _interactionSettings->feedback = feedback;

  return Error(Error::OK);
}

std::shared_ptr<InteractionSettings> InteractionSettingsConfig::getSettings() {
  return _interactionSettings;
}

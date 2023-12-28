#include "InteractionSettingsConfig.h"

InteractionSettingsConfig::InteractionSettingsConfig(
    std::shared_ptr<IFileHandler> fileHandler)
    : BaseConfig(fileHandler) {}

Error InteractionSettingsConfig::save(const std::string &filename) const {
  // TODO: Implement this function
  return Error(Error::OK);
}

Error InteractionSettingsConfig::parseJson(const DynamicJsonDocument &doc) {
  if (!doc.containsKey("buttonInteraction")) {
    return Error(Error::InteractionSettingsButtonInteractionKeyMissing);
  }

  const JsonObjectConst buttonInteraction = doc["buttonInteraction"];

  if (!buttonInteraction.containsKey("buttons")) {
    return Error(Error::InteractionSettingsButtonsSubkeyMissing);
  } else {
    Error err =
        parseButtonSettings(buttonInteraction["buttons"].as<JsonObjectConst>());
    if (err)
      return err;
  }

  if (!buttonInteraction.containsKey("beepSettings")) {
    return Error(Error::InteractionSettingsBeepSettingsSubkeyMissing);
  } else {
    Error err = parseBeepSettings(
        buttonInteraction["beepSettings"].as<JsonObjectConst>());
    if (err)
      return err;
  }

  if (!buttonInteraction.containsKey("feedback")) {
    return Error(Error::InteractionSettingsFeedbackSubkeyMissing);
  } else {
    Error err = parseFeedbackSettings(
        buttonInteraction["feedback"].as<JsonObjectConst>());
    if (err)
      return err;
  }

  return Error(Error::OK);
}

Error InteractionSettingsConfig::parseButtonSettings(
    const JsonObjectConst &buttonObj) {
  for (auto buttonPair : buttonObj) {
    const std::string buttonKey = buttonPair.key().c_str();
    const JsonObjectConst buttonSettingsObj =
        buttonPair.value().as<JsonObjectConst>();

    InteractionSettings::Button button;
    button.description = buttonSettingsObj["description"].as<std::string>();

    // Parsing 'autoRepeat' settings
    const JsonObjectConst autoRepeatObj =
        buttonSettingsObj["autoRepeat"].as<JsonObjectConst>();
    InteractionSettings::AutoRepeat autoRepeat;
    autoRepeat.initialDelayMs = autoRepeatObj["initialDelayMs"];
    autoRepeat.standardRateMs = autoRepeatObj["standardRateMs"];

    const JsonObjectConst accelerationObj =
        autoRepeatObj["acceleration"].as<JsonObjectConst>();
    autoRepeat.acceleration.startAfterMs = accelerationObj["startAfterMs"];
    autoRepeat.acceleration.rateDecreaseIntervalMs =
        accelerationObj["rateDecreaseIntervalMs"];
    autoRepeat.acceleration.minimumRateMs = accelerationObj["minimumRateMs"];

    button.autoRepeat = autoRepeat;
    _interactionSettings.buttons[buttonKey] = button;
  }

  return Error(Error::OK);
}

Error InteractionSettingsConfig::parseBeepSettings(
    const JsonObjectConst &beepSettings) {
  InteractionSettings::BeepSettings settings;
  settings.description = beepSettings["description"].as<std::string>();
  settings.standardFrequency = beepSettings["standardFrequency"];
  settings.standardDurationMs = beepSettings["standardDurationMs"];

  const JsonObjectConst limitPattern = beepSettings["limitReachedPattern"];
  settings.limitReachedPattern.description =
      limitPattern["description"].as<std::string>();
  settings.limitReachedPattern.frequency = limitPattern["frequency"];
  settings.limitReachedPattern.durationMs = limitPattern["durationMs"];
  settings.limitReachedPattern.pattern =
      limitPattern["pattern"].as<std::string>();

  _interactionSettings.beepSettings = settings;

  return Error(Error::OK);
}

Error InteractionSettingsConfig::parseFeedbackSettings(
    const JsonObjectConst &feedbackSettings) {
  InteractionSettings::Feedback feedback;
  feedback.description = feedbackSettings["description"].as<std::string>();
  feedback.onButtonRelease = feedbackSettings["onButtonRelease"];
  feedback.audioFeedbackOnLimit = feedbackSettings["audioFeedbackOnLimit"];

  _interactionSettings.feedback = feedback;

  return Error(Error::OK);
}

InteractionSettings InteractionSettingsConfig::getSettings() {
  return _interactionSettings;
}
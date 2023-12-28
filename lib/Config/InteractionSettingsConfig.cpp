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

  parseCommonSettings(buttonInteraction);

  if (buttonInteraction.containsKey("buttons")) {
    Error err =
        parseButtonSettings(buttonInteraction["buttons"].as<JsonObjectConst>());
    if (err)
      return err;
  } else {
    return Error(Error::InteractionSettingsButtonsSubkeyMissing);
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


void InteractionSettingsConfig::parseCommonSettings(
    const JsonObjectConst &buttonInteraction) {
  // Parse common settings
  if (buttonInteraction.containsKey("commonSettings")) {
    const JsonObjectConst commonSettings = buttonInteraction["commonSettings"];
    _interactionSettings.commonSettings.debounceMs =
        commonSettings["debounceMs"];

    // Parse autorepeat settings
    if (commonSettings.containsKey("autoRepeat")) {
      const JsonObjectConst autoRepeatObj =
          commonSettings["autoRepeat"].as<JsonObjectConst>();
      _interactionSettings.commonSettings.autoRepeat.initialDelayMs =
          autoRepeatObj["initialDelayMs"];
      _interactionSettings.commonSettings.autoRepeat.standardRateMs =
          autoRepeatObj["standardRateMs"];
      _interactionSettings.commonSettings.autoRepeat.acceleration.startAfterMs =
          autoRepeatObj["acceleration"]["startAfterMs"];
      _interactionSettings.commonSettings.autoRepeat.acceleration
          .rateDecreaseIntervalMs =
          autoRepeatObj["acceleration"]["rateDecreaseIntervalMs"];
      _interactionSettings.commonSettings.autoRepeat.acceleration
          .minimumRateMs = autoRepeatObj["acceleration"]["minimumRateMs"];
    }
  }
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
    if (buttonSettingsObj.containsKey("autoRepeat") &&
        buttonSettingsObj["autoRepeat"].as<bool>()) {
      button.hasAutoRepeat = true;
    } else {
      button.hasAutoRepeat = false;
    }

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
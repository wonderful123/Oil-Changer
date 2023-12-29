// 2. Test Failure to Load Non-Existent Interaction Settings
// Test Name: testLoadInteractionSettingsFailure
// Objective: Ensure that an appropriate error is returned when the JSON file
// does not exist. Expected Result: The method returns an error indicating a
// file loading failure.
// 3. Test Parsing of Interaction Settings
// Test Name: testParseInteractionSettings
// Objective: Ensure that the JSON content is correctly parsed into the internal
// data structure. Expected Result: After parsing, the internal data structure
// accurately represents the settings defined in the JSON file.
// 4. Test Saving of Interaction Settings
// Test Name: testSaveInteractionSettings
// Objective: Ensure that modified interaction settings are correctly saved back
// to a JSON file. Expected Result: The JSON file is updated to reflect the
// changes made to the interaction settings.
// 5. Test Access and Modification of Settings
// Test Name: testAccessAndModifySettings
// Objective: Ensure that the settings can be accessed and modified
// programmatically. Expected Result: After modification, the internal data
// structure reflects the changes, and these changes are persistent when saved
// and reloaded.
// 6. Test Error Handling in Interaction Settings Parsing
// Test Name: testInteractionSettingsParsingErrorHandling
// Objective: Ensure that parsing errors (like missing fields, wrong data types)
// are correctly handled. Expected Result: The method returns an error
// indicating a parsing issue, and the internal state remains valid or indicates
// an error state.
// 7. Test Initialization Without Config File
// Test Name: testInitializationWithoutConfigFile
// Objective: Ensure that the system can handle the scenario where the config
// file is not present initially. Expected Result: The system either creates a
// default configuration or handles the absence gracefully.
// 8. Test Real-time Update of Settings
// Test Name: testRealTimeUpdateOfSettings
// Objective: Ensure that changes to settings are reflected in real-time in the
// system's behavior. Expected Result: Upon changing a setting, the
// corresponding system behavior changes accordingly without needing a restart
// or reload.

#include "InteractionSettingsConfig.h"
#include "../test_utils.h"
#include "Error.h"
#include "IFileHandler.h"
#include "Mocks/MockFileHandler.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class InteractionSettingsConfigTest : public ::testing::Test {
protected:
  std::shared_ptr<MockFileHandler> mockFileHandler;
  std::shared_ptr<InteractionSettingsConfig> config;
  const std::string DUMMY_FILE_PATH = "dummy_config.json";

  InteractionSettingsConfigTest() {
    mockFileHandler = std::make_shared<MockFileHandler>();
    config = std::make_shared<InteractionSettingsConfig>(mockFileHandler);
  }
};

TEST_F(InteractionSettingsConfigTest, LoadDeserializationFailure) {
  const std::string invalidMockFileContent = "This is not a JSON content";

  expectOpenReadCloseForContent(*mockFileHandler, invalidMockFileContent,
                                DUMMY_FILE_PATH);

  Error loadError = config->load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::JsonInputInvalid)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(InteractionSettingsConfigTest, NonExistantFileLoadFailure) {
  // Setup the mock to simulate a file open failure
  EXPECT_CALL(*mockFileHandler, open(DUMMY_FILE_PATH, "r"))
      .WillOnce(testing::Return(false)); // Simulate file open failure

  // Attempt to load the non-existent file
  Error loadError = config->load(DUMMY_FILE_PATH);

  // Check if the appropriate error code is returned
  EXPECT_EQ(loadError, Error::FileOpenFailure)
      << "Expected a file open failure error, but got: "
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(InteractionSettingsConfigTest, ParseValidButtonSettings) {
  // Setup the mock to simulate reading valid button settings from a file
  const std::string validButtonSettings = R"({
{
  "buttonInteraction": {
    "description": "Configuration for button behaviors and interactions",
    "commonSettings": {
      "debounceMs": 150,
      "autoRepeat": {
        "description": "Auto-repeat behavior on long press",
        "initialDelayMs": 1000,
        "standardRateMs": 100,
        "acceleration": {
          "description": "Acceleration parameters for auto-repeat",
          "startAfterMs": 2000,
          "rateDecreaseIntervalMs": 500,
          "minimumRateMs": 10
        }
      }
    },
    "buttons": {
      "ButtonPlus": {
        "description": "Settings for increment (plus) button",
        "autoRepeat": true
      },
      "ButtonMinus": {
        "description": "Settings for decrement (minus) button",
        "autoRepeat": true
      },
      "ButtonManualExtract": {
        "description": "Settings for manual extract button"
      },
      "ButtonManualFill": {
        "description": "Settings for manual fill button"
      },
      "ButtonStart": {
        "description": "Settings for start button",
        "autoRepeat": true
      },
      "ButtonStop": {
        "description": "Settings for stop button"
      }
    },
    "beepSettings": {
      "description": "Audio feedback settings for button interactions",
      "standardFrequency": 2731,
      "standardDurationMs": 150,
      "limitReachedPattern": {
        "description": "Special beep pattern when a limit is reached",
        "frequency": 3500,
        "durationMs": 250,
        "pattern": "double-beep"
      }
    },
    "feedback": {
      "description": "General feedback settings",
      "onButtonRelease": false,
      "audioFeedbackOnLimit": true
    }
  }
}
    })";
  expectOpenReadCloseForContent(*mockFileHandler, validButtonSettings,
                                DUMMY_FILE_PATH);

  // Load the settings
  Error loadError = config->load(DUMMY_FILE_PATH);
  EXPECT_EQ(loadError, Error::OK)
      << "Failed to load valid button settings: "
      << loadError.getFormattedMessage(loadError.code());

    // Validate that the button settings are correctly parsed
  auto settings = config->getSettings();

  // Common settings validation
  EXPECT_EQ(settings.commonSettings.debounceMs, 150);

  // Auto-repeat settings validation
  EXPECT_EQ(settings.commonSettings.autoRepeat.initialDelayMs, 1000);
  // ... validate other auto-repeat settings ...

  // Button specific settings validation
  EXPECT_TRUE(settings.buttons["ButtonPlus"].hasAutoRepeat);
  EXPECT_TRUE(settings.buttons["ButtonMinus"].hasAutoRepeat);
  EXPECT_FALSE(
      settings.buttons["ButtonManualExtract"]
          .hasAutoRepeat); // Assuming default is false if not specified
  // ... validate other buttons ...

  // Beep settings validation
  EXPECT_EQ(settings.beepSettings.standardFrequency, 2731);
  // ... validate other beep settings ...

  // Feedback settings validation
  EXPECT_FALSE(settings.feedback.onButtonRelease);
  EXPECT_TRUE(settings.feedback.audioFeedbackOnLimit);
}

TEST_F(InteractionSettingsConfigTest, MissingButtonSettingsFailure) {
  // Setup the mock to simulate reading JSON content without button settings
  const std::string missingButtonSettings = R"({
        "buttonInteraction": {
        }
    })";
  expectOpenReadCloseForContent(*mockFileHandler, missingButtonSettings,
                                DUMMY_FILE_PATH);

  // Load the settings
  Error loadError = config->load(DUMMY_FILE_PATH);

  // Check if the appropriate error code is returned
  EXPECT_EQ(loadError, Error::InteractionSettingsButtonsSubkeyMissing)
      << "Expected an error for missing button settings, but got: "
      << loadError.getFormattedMessage(loadError.code());
}

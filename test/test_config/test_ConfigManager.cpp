#include "ConfigManager.h"
#include "Mocks/MockFileHandler.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;

/************************** Test Summary ****************************

Constructor Tests:
- ConstructorInitialization: Verifies that the ConfigManager constructor
correctly initializes its internal state, particularly the file handler and any
initial configurations.

LoadConfig Tests:
- LoadHardwareConfigSuccess: Tests loading a valid HardwareConfig. Verifies that
the method returns Error::OK and the correct configuration is loaded and
accessible.
- LoadHardwareConfigFailure: Tests behavior when loading a non-existent or
invalid HardwareConfig file. Ensures that an appropriate error (e.g.,
Error::FileOpenFailure or Error::JsonInputInvalid) is returned.
- ConfigTypeNotRecognized: Tries to load a configuration of an unrecognized type
and checks if Error::ConfigTypeNotRecognized is returned.
- FileHandlerErrorHandling: Simulates a failure in the file handler (e.g., file
not opening) and checks if the ConfigManager correctly returns an error.
- JsonDeserializationError: Tests the scenario where JSON deserialization fails
due to an invalid format, ensuring that the appropriate error is returned.

GetHardwareConfig Tests:
- GetHardwareConfigExisting: Tests retrieving a previously loaded HardwareConfig
and verifies the integrity of the returned object.
- GetHardwareConfigNonExisting: Attempts to retrieve a non-existent
HardwareConfig and checks if a nullptr is returned.

ErrorHandling Tests:
- NoConfigLoadedError: Attempts to access a configuration when none is loaded,
verifying that the correct error is returned or indicated.
- InternalStateConsistency: Ensures that failed operations do not leave the
ConfigManager in an inconsistent state.

********************************************************************/

class ConfigManagerTest : public ::testing::Test {
protected:
  MockFileHandler mockFileHandler;
  ConfigManager configManager;

  ConfigManagerTest() : configManager(&mockFileHandler) {}

  const std::string validHardwareConfigJSON = R"json({
      "components": {
        "singlePin": [{
          "id": "FlowMeterExtract",
          "type": "FlowMeter",
          "pinNumber": 15,
          "options": {
            "pcntUnit": "PCNT_UNIT_1",
            "pulseMultiplier": 100,
            "filterValue": 10,
            "filterEnabled": true
          }
        }],
        "multiPin": [{
          "id": "SPI",
          "type": "SPI",
          "pins": {
            "MOSI": 23,
            "MISO": 19,
            "SCLK": 18,
            "CS": 5
          }
        }]
      }
    })json";

  void SetUp() override {
  }
};

TEST_F(ConfigManagerTest, LoadHardwareConfigSuccess) {
  // Arrange
  EXPECT_CALL(mockFileHandler, open(_, _)).WillOnce(Return(true));
  EXPECT_CALL(mockFileHandler, read())
      .WillOnce(Return(validHardwareConfigJSON));
  // Act
  Error result = configManager.loadConfig("HardwareConfig");

  // Assert
  EXPECT_EQ(result.code(), Error::OK);
}

TEST_F(ConfigManagerTest, LoadHardwareConfigFailure) {
  // Arrange
  EXPECT_CALL(mockFileHandler, open(_, _))
      .WillOnce(Return(false)); // Simulate file open failure

  // Act
  Error result = configManager.loadConfig("HardwareConfig");

  // Assert
  EXPECT_EQ(result.code(), Error::FileOpenFailure)
      << result.getFormattedMessage(result.code());
  ;
}

TEST_F(ConfigManagerTest, ConfigTypeNotRecognized) {
  // Act
  Error result = configManager.loadConfig("UnknownConfigType");

  // Assert
  EXPECT_EQ(result.code(), Error::ConfigTypeNotRecognized)
      << result.getFormattedMessage(result.code());
}

TEST_F(ConfigManagerTest, FileHandlerErrorHandling) {
  // Arrange
  EXPECT_CALL(mockFileHandler, open(_, _)).WillOnce(Return(true));
  EXPECT_CALL(mockFileHandler, read())
      .WillOnce(Return("")); // Simulate empty or invalid file content

  // Act
  Error result = configManager.loadConfig("HardwareConfig");

  // Assert
  EXPECT_NE(result.code(), Error::OK)
      << result.getFormattedMessage(result.code());
  // Expecting an error but not specifying which one
}

TEST_F(ConfigManagerTest, JsonDeserializationError) {
  // Arrange
  std::string invalidJson = "invalid_json"; // Not a valid JSON format
  EXPECT_CALL(mockFileHandler, open(_, _)).WillOnce(Return(true));
  EXPECT_CALL(mockFileHandler, read()).WillOnce(Return(invalidJson));

  // Act
  Error result = configManager.loadConfig("HardwareConfig");

  // Assert
  EXPECT_NE(result.code(), Error::OK)
      << result.getFormattedMessage(result.code());
  // Expecting a JSON-related error but not specifying which one
}

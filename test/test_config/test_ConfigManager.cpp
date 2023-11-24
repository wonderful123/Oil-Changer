#include "ConfigManager.h"
#include "Mocks/MockFileHandler.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;

/************************** Test Summary ****************************/
/*

  Constructor and Destructor Tests:
    - ConstructorInitialization: Verifies that the ConfigManager constructor
  initializes the internal structures correctly.

  getHardwareConfig Method Tests:
    - RetrieveExistingConfig: Tests retrieving a previously loaded hardware
  configuration, expecting a valid object.
    - RetrieveNonExistingConfig: Verifies the behavior when attempting to
  retrieve a hardware configuration that does not exist (should return nullptr).

  loadConfig Method Tests:
    - LoadSuccess: Tests successful loading of a valid hardware configuration
  file, expecting no errors and correct parsing.
    - LoadUnrecognizedConfigType: Validates that an appropriate error is
  returned when trying to load a configuration of an unrecognized type.
    - LoadFileReadFailure: Simulates a file read failure to test error handling
  in the loadConfig method.
    - LoadDeserializationFailure: Validates proper error handling when loading
  content that cannot be deserialized as JSON.

*/
/*******************************************************************/

class ConfigManagerTest : public ::testing::Test {
protected:
  MockFileHandler mockFileHandler;
  ConfigManager configManager;

  ConfigManagerTest() : configManager(&mockFileHandler) {}

  void SetUp() override {
    // Set up any required configurations for each test
  }
};

TEST_F(ConfigManagerTest, ConstructorInitialization) {
  // Assuming ConfigManager's constructor initializes an internal structure,
  // such as the 'configs' unordered_map, we need to verify it's in a valid
  // state. As this is an internal detail, it might require making the test
  // class a friend of ConfigManager, or using some indirect method to verify
  // the initialization.

  // Here we'll use a simple indirect method: attempting to retrieve a config
  // and expecting it to return nullptr, indicating the map is initialized but
  // empty.
  auto hardwareConfig = configManager.getHardwareConfig();
  EXPECT_EQ(hardwareConfig, nullptr);
}

TEST_F(ConfigManagerTest, RetrieveExistingConfig) {
  // Set up the mock to simulate successful file opening
  EXPECT_CALL(mockFileHandler, open("hardwareConfig.json", "r"))
      .WillOnce(Return(true));

  // Mock the file handler to return a valid configuration JSON upon reading
  EXPECT_CALL(mockFileHandler, read()).WillOnce(Return(R"({
    "gpioPins": [
      {"pinNumber": 1, "id": "pin1", "type": "Digital"}
    ]
  })"));

  // Load a valid hardware configuration
  Error loadError =
      configManager.loadConfig("HardwareConfig");
  EXPECT_EQ(loadError.code(), Error::OK)
      << loadError.getFormattedMessage(loadError.code());
  
  // Retrieve the loaded hardware configuration
  auto hardwareConfig = configManager.getHardwareConfig();
  EXPECT_NE(hardwareConfig, nullptr);
}

TEST_F(ConfigManagerTest, RetrieveNonExistingConfig) {
  // Attempt to retrieve the hardware configuration when none has been loaded.
  // This should return nullptr.
  auto nonExistingConfig = configManager.getHardwareConfig();
  EXPECT_EQ(nonExistingConfig, nullptr);
}

TEST_F(ConfigManagerTest, LoadSuccess) {
  // Set up the mock to simulate successful file opening
  EXPECT_CALL(mockFileHandler, open("hardwareConfig.json", "r"))
      .WillOnce(Return(true));

  // Mock file handler to simulate reading a correct config file
  EXPECT_CALL(mockFileHandler, read()).WillOnce(Return(R"({
    "gpioPins": [
      {"pinNumber": 12, "id": "BUZZER", "type": "Buzzer"},
      {"pinNumber": 1, "id": "pin1", "type": "Digital"}
    ]
  })"));

  Error loadError =
      configManager.loadConfig("HardwareConfig");
  EXPECT_EQ(loadError.code(), Error::OK)
      << loadError.getFormattedMessage(loadError.code());

  auto hardwareConfig = configManager.getHardwareConfig();
  EXPECT_NE(hardwareConfig, nullptr);

  // Additional checks on hardwareConfig
  const auto &gpioConfigs = hardwareConfig->getHardwarePinConfigs();
  EXPECT_EQ(gpioConfigs.size(), 2); // Check the number of configured GPIO pins

  // Further checks can be added to validate individual GPIO pin configurations
  EXPECT_EQ(gpioConfigs[0].pinNumber, 12);
  EXPECT_EQ(gpioConfigs[0].id, "BUZZER");
  EXPECT_EQ(gpioConfigs[0].type, "Buzzer");

  EXPECT_EQ(gpioConfigs[1].pinNumber, 1);
  EXPECT_EQ(gpioConfigs[1].id, "pin1");
  EXPECT_EQ(gpioConfigs[1].type, "Digital");
}

TEST_F(ConfigManagerTest, LoadUnrecognizedConfigType) {
  Error loadError =
      configManager.loadConfig("UnknownConfigType");

  EXPECT_EQ(loadError.code(), Error::ConfigTypeNotRecognized)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigManagerTest, LoadFileReadFailure) {
  // Set expectations for open and read calls
  EXPECT_CALL(mockFileHandler, open("hardwareConfig.json", "r"))
      .WillOnce(Return(true));
  EXPECT_CALL(mockFileHandler, read()).WillOnce(Return(std::string("")));
  EXPECT_CALL(mockFileHandler, close())
      .Times(testing::AtLeast(0)); // Handle close call

  Error loadError =
      configManager.loadConfig("HardwareConfig");

  // Expecting an empty JSON input error
  EXPECT_EQ(loadError.code(), Error::JsonInputEmpty)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigManagerTest, LoadDeserializationFailure) {
  // Simulate file handler behavior for an invalid JSON content
  EXPECT_CALL(mockFileHandler, open("hardwareConfig.json", "r"))
      .WillOnce(Return(true));
  EXPECT_CALL(mockFileHandler, read()).WillOnce(Return("invalid JSON content"));
  EXPECT_CALL(mockFileHandler, close())
      .Times(testing::AtLeast(0)); // Handle close call

  Error loadError =
      configManager.loadConfig("HardwareConfig");

  // Check for the appropriate error code for invalid JSON input
  EXPECT_EQ(loadError.code(), Error::JsonInputInvalid)
      << loadError.getFormattedMessage(loadError.code());
}

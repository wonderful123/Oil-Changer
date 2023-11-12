#include "mock_IFileHandler.h"
#include <Config.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Test Summary:
// - Load and validate configurations with various scenarios:
//   - Invalid content
//   - File does not exist
//   - Empty content
//   - Partial content
//   - Non-JSON content
//   - Large content
//   - Missing 'gpioPins' array key
//   - Missing GPIO pin number
//   - Missing GPIO pin type
//   - Missing GPIO ID
//   - Missing resolution for ADC
//   - Get configuration for a component without options
//   - Load configuration with many different GPIO configurations
//   - Get configuration for a component with options
//   - Ensure file closure after successful processing
//   - Ensure file closure on deserialization error
//   - Ensure no file closure when open fails

class ConfigTest : public ::testing::Test {
protected:
  ConfigTest() : config(&mockFileHandler) {} // Initialize config here

  static const std::string TEST_FILE_PATH;

  MockFileHandler mockFileHandler;
  Config config;
};

const std::string ConfigTest::TEST_FILE_PATH = "test_config.json";

void expectOpenReadCloseForContent(MockFileHandler &mockFileHandler,
                                   const std::string &content,
                                   const std::string &filePath) {
  EXPECT_CALL(mockFileHandler, open(filePath, "r"))
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(mockFileHandler, read()).WillOnce(::testing::Return(content));
  EXPECT_CALL(mockFileHandler, close()).Times(1);
}

TEST_F(ConfigTest, LoadConfigFileWithInvalidContent) {
  const std::string invalidMockFileContent = R"({ "someKey": "someValue", })";

  expectOpenReadCloseForContent(mockFileHandler, invalidMockFileContent,
                                TEST_FILE_PATH);

  Error loadError = config.load(TEST_FILE_PATH);

  EXPECT_EQ(loadError, Error::JsonDeserializationError)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileDoesNotExist) {
  // Arrange
  const std::string nonExistentFilePath = "nonexistent_config.json";

  // Expect the file handler's open method to be called with the non-existent
  // file path and return false to simulate the file doesn't exist or can't be
  // opened.
  EXPECT_CALL(mockFileHandler, open(nonExistentFilePath, "r"))
      .WillOnce(::testing::Return(false));

  // Act
  Error loadError = config.load(nonExistentFilePath);

  // Assert
  EXPECT_EQ(loadError, Error::FileNotOpen)
      << loadError.getFormattedMessage(loadError.code());
  // Expecting load to return an error
  // because the file doesn't exist
}

TEST_F(ConfigTest, LoadConfigFileEmptyContent) {
  // Arrange
  const std::string emptyMockFileContent = "";

  expectOpenReadCloseForContent(mockFileHandler, emptyMockFileContent,
                                TEST_FILE_PATH);

  // Act
  Error loadError = config.load(TEST_FILE_PATH);

  // Assert
  EXPECT_EQ(loadError, Error::FileReadError)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileWithPartialContent) {
  // Arrange
  const std::string partialMockFileContent =
      R"({ "someKey": )"; // Incomplete JSON content

  expectOpenReadCloseForContent(mockFileHandler, partialMockFileContent,
                                TEST_FILE_PATH);

  // Act
  Error loadError = config.load(TEST_FILE_PATH);

  // Assert
  EXPECT_EQ(loadError, Error::JsonDeserializationError)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileWithNonJSONContent) {
  // Arrange
  const std::string nonJsonMockFileContent = "This is not a JSON content";

  expectOpenReadCloseForContent(mockFileHandler, nonJsonMockFileContent,
                                TEST_FILE_PATH);

  // Act
  Error loadError = config.load(TEST_FILE_PATH);

  // Assert
  EXPECT_EQ(loadError, Error::JsonDeserializationError)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileWithLargeContent) {
  // Arrange
  // Construct a JSON string larger than the StaticJsonDocument buffer size
  const std::string largeMockFileContent =
      R"({ "key": ")" + std::string(5000, 'a') + R"(" })";

  expectOpenReadCloseForContent(mockFileHandler, largeMockFileContent,
                                TEST_FILE_PATH);

  // Act
  Error loadError = config.load(TEST_FILE_PATH);

  // Assert
  EXPECT_EQ(loadError, Error::JsonDeserializationError)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileWithoutGpioPinsArrayKey) {
  // Arrange
  const std::string mockFileContentWithoutGpioPinsArrayKey = R"(
    {
      "hardware": "ESP32"
    }
  )";

  expectOpenReadCloseForContent(
      mockFileHandler, mockFileContentWithoutGpioPinsArrayKey, TEST_FILE_PATH);

  // Act
  Error loadError = config.load(TEST_FILE_PATH);

  // Assert
  EXPECT_EQ(loadError, Error::ConfigUndefinedGpioPinsArrayKey)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileWithMissingGpioPinNumberKey) {
  const std::string contentMissingGpio = R"({
    "hardware": "ESP32",
    "gpioPins": [{
      "id": "BUZZER",
      "type": "PWM",
      "options": {}
    }]
  })";

  expectOpenReadCloseForContent(mockFileHandler, contentMissingGpio,
                                TEST_FILE_PATH);

  // Act
  Error loadError = config.load(TEST_FILE_PATH);

  // Assert
  EXPECT_EQ(loadError, Error::ConfigUndefinedGpioKeyForPin)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileWithMissingGpioPinType) {
  const std::string contentMissingType = R"({
    "hardware": "ESP32",
    "gpioPins": [{
      "gpio": 12,
      "id": "BUZZER",
      "options": {}
    }]
  })";

  expectOpenReadCloseForContent(mockFileHandler, contentMissingType,
                                TEST_FILE_PATH);

  // Act
  Error loadError = config.load(TEST_FILE_PATH);

  // Assert
  EXPECT_EQ(loadError, Error::ConfigMissingGpioPinType)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileWithMissingGpioId) {
  const std::string contentMissingId = R"({
    "hardware": "ESP32",
    "gpioPins": [{
      "gpio": 12,
      "type": "PWM",
      "options": {}
    }]
  })";

  expectOpenReadCloseForContent(mockFileHandler, contentMissingId,
                                TEST_FILE_PATH);

  Error loadError = config.load(TEST_FILE_PATH);

  EXPECT_EQ(loadError, Error::ConfigMissingGpioPinId)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, LoadConfigFileWithMissingResolutionForADC) {
  const std::string contentMissingResolution = R"({
    "hardware": "ESP32",
    "gpioPins": [{
      "gpio": 13,
      "id": "VOLTAGE_SENSE",
      "type": "ADC",
      "options": {}
    }]
  })";

  expectOpenReadCloseForContent(mockFileHandler, contentMissingResolution,
                                TEST_FILE_PATH);

  Error loadError = config.load(TEST_FILE_PATH);
  EXPECT_EQ(loadError, Error::ConfigMissingADCResolutionOption)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, GetConfigForComponentWithoutOptions) {
  // Arrange
  const std::string mockFileContent = R"({
        "hardware": "ESP32",
        "gpioPins": [
            {
                "gpio": 12,
                "id": "BUZZER",
                "type": "PWM"
            }
        ]
    })";

  expectOpenReadCloseForContent(mockFileHandler, mockFileContent,
                                TEST_FILE_PATH);
  Error loadError = config.load(TEST_FILE_PATH);

  // Act
  ComponentConfig buzzerConfig = config.getConfigFor("BUZZER");

  // Assert
  ASSERT_EQ(loadError, Error::OK)
      << loadError.getFormattedMessage(loadError.code());

  ASSERT_FALSE(buzzerConfig.type.empty()); // Check if the config is valid
  EXPECT_EQ(buzzerConfig.gpio, 12);
  EXPECT_EQ(buzzerConfig.type, "PWM");
  EXPECT_TRUE(buzzerConfig.options.empty()); // Ensure there are no options
}

TEST_F(ConfigTest, LoadConfigFileWithManyDifferentGPIOConfigurations) {
  // Arrange
  const std::string mockFileContent = R"(
    {
      "hardware": "ESP32",
      "gpioPins": [{
        "gpio": 12,
        "id": "BUZZER",
        "type": "PWM",
        "options": {
          "volume": "10",
          "buttonPressFrequency": "900",
          "buttonHoldFrequency": "1100",
          "buttonPressDuration": "100",
          "buttonHoldDuration": "50"
        }
      }, {
        "gpio": 13,
        "id": "VOLTAGE_SENSE",
        "type": "ADC",
        "options": {
          "resolution": "4096",
          "attenuation": "11"
        }
      }, {
        "gpio": 14,
        "id": "FLOW_METER_FILL",
        "type": "DIGITAL",
        "options": {}
      }]
    }
  )";

  expectOpenReadCloseForContent(mockFileHandler, mockFileContent,
                                TEST_FILE_PATH);

  // Act
  Error loadError = config.load(TEST_FILE_PATH);

  // Assert
  EXPECT_EQ(loadError, Error::OK)
      << loadError.getFormattedMessage(loadError.code());

  // Assert for BUZZER configuration
  ComponentConfig buzzerConfig = config.getConfigFor("BUZZER");
  ASSERT_FALSE(buzzerConfig.type.empty());
  EXPECT_EQ(buzzerConfig.gpio, 12);
  EXPECT_EQ(buzzerConfig.type, "PWM");
  ASSERT_FALSE(buzzerConfig.options.empty());
  EXPECT_EQ(buzzerConfig.options.at("volume"), "10");
  EXPECT_EQ(buzzerConfig.options.at("buttonPressFrequency"), "900");
  EXPECT_EQ(buzzerConfig.options.at("buttonPressDuration"), "100");
  EXPECT_EQ(buzzerConfig.options.at("buttonHoldDuration"), "50");

  // Assert for VOLTAGE_SENSE configuration
  ComponentConfig voltageSenseConfig = config.getConfigFor("VOLTAGE_SENSE");
  ASSERT_FALSE(voltageSenseConfig.type.empty());
  EXPECT_EQ(voltageSenseConfig.gpio, 13);
  EXPECT_EQ(voltageSenseConfig.type, "ADC");
  ASSERT_FALSE(voltageSenseConfig.options.empty());
  EXPECT_EQ(voltageSenseConfig.options.at("resolution"), "4096");
  EXPECT_EQ(voltageSenseConfig.options.at("attenuation"), "11");

  // Assert for FLOW_METER_FILL configuration
  ComponentConfig flowMeterFillConfig = config.getConfigFor("FLOW_METER_FILL");
  ASSERT_FALSE(flowMeterFillConfig.type.empty());
  EXPECT_EQ(flowMeterFillConfig.gpio, 14);
  EXPECT_EQ(flowMeterFillConfig.type, "DIGITAL");
  EXPECT_TRUE(flowMeterFillConfig.options.empty()); // No options expected
}

TEST_F(ConfigTest, GetConfigForComponentWithOptions) {
  // Arrange
  const std::string mockFileContent = R"({
    "hardware": "ESP32",
    "gpioPins": [
      {
        "gpio": 13,
        "id": "VOLTAGE_SENSE",
        "type": "ADC",
        "options": {
          "resolution": 12,
          "attenuation": 11
        }
      }
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, mockFileContent,
                                TEST_FILE_PATH);
  Error loadError = config.load(TEST_FILE_PATH);

  // Act
  auto voltageSenseConfig = config.getConfigFor("VOLTAGE_SENSE");

  // Assert
  ASSERT_EQ(loadError, Error::OK);
  ASSERT_FALSE(voltageSenseConfig.type.empty()); // Check if the config is valid

  EXPECT_EQ(voltageSenseConfig.gpio, 13);
  EXPECT_EQ(voltageSenseConfig.type, "ADC");
  ASSERT_FALSE(
      voltageSenseConfig.options.empty()); // Ensure options are present

  // Check the specific options
  EXPECT_EQ(std::stoi(voltageSenseConfig.options.at("resolution")), 12);
  EXPECT_EQ(std::stoi(voltageSenseConfig.options.at("attenuation")), 11);
}

TEST_F(ConfigTest, EnsureFileIsClosedAfterSuccessfulProcessing) {
  const std::string validContent = R"({
    "hardware": "ESP32",
    "gpioPins": [
        {
            "gpio": 12,
            "id": "BUZZER",
            "type": "PWM",
            "options": {
                "volume": 10,
                "buttonPressFrequency": 900,
                "buttonHoldFrequency": 1100,
                "buttonPressDuration": 100,
                "buttonHoldDuration": 50
            }
        },
        {
            "gpio": 13,
            "id": "VOLTAGE_SENSE",
            "type": "ADC",
            "options": {
                "resolution": 4096,
                "attenuation": 11
            }
        },
        {"gpio": 14, "id": "FLOW_METER_FILL", "type": "DIGITAL", "options": {}},
        {"gpio": 15, "id": "FLOW_METER_EXTRACT", "type": "DIGITAL", "options": {}}
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, validContent, TEST_FILE_PATH);
  Error loadError = config.load(TEST_FILE_PATH);

  EXPECT_EQ(loadError, Error::OK)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, EnsureFileIsClosedOnDeserializationError) {
  const std::string invalidJsonContent =
      R"({ "hardware": "ESP32" )"; // Incomplete JSON

  expectOpenReadCloseForContent(mockFileHandler, invalidJsonContent,
                                TEST_FILE_PATH);
  Error loadError = config.load(TEST_FILE_PATH);

  EXPECT_EQ(loadError, Error::JsonDeserializationError)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(ConfigTest, EnsureFileIsNotClosedWhenOpenFails) {
  EXPECT_CALL(mockFileHandler,
              open(TEST_FILE_PATH, "r")) // The mode should be specified
      .WillOnce(::testing::Return(false));

  // Ensure close() is never called if open() fails
  EXPECT_CALL(mockFileHandler, close()).Times(0);

  Error loadError = config.load(TEST_FILE_PATH);
  EXPECT_EQ(loadError, Error::FileNotOpen)
      << loadError.getFormattedMessage(loadError.code());
}

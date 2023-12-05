#include "../test_utils.h"
#include "DIContainer.h"
#include "Mocks/MockFileHandler.h"
#include <HardwareConfig.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/************************** Test Summary ****************************

  Constructor Tests:
    - ConstructorInitialization: Verifies that the constructor correctly initializes the HardwareConfig object.

  Load Tests:
    - LoadDeserializationFailure: Validates proper error handling when loading content that cannot be deserialized as JSON.
    - EmptyConfiguration: Checks the behavior when the JSON file is valid but contains no configuration data.
    - InvalidConfigurationStructure: Ensures error handling for JSON files with valid syntax but incorrect structure.

  Parsing Configuration Tests:
    - ParseSinglePinConfiguration: Tests the parsing logic for single-pin configurations to ensure correct data extraction from JSON.
    - ParseMultiPinConfiguration: Validates the parsing of multi-pin configurations from JSON.
    - OptionParsing: Checks if options within configurations are parsed and stored correctly.

  getHardwarePinConfigs Method Tests:
    - CorrectPinConfigsRetrieval: Ensures that getHardwarePinConfigs returns the correct list of pin configurations.

*******************************************************************/

using ::testing::_;
using ::testing::Return;

class HardwareConfigTest : public ::testing::Test {
protected:
  std::shared_ptr<MockFileHandler> mockFileHandler;
  std::shared_ptr<HardwareConfig> hardwareConfig;
  const std::string DUMMY_FILE_PATH = "dummy_config.json";

  HardwareConfigTest() {
    // Initialize DI Container with mock file handler
    DIContainer::clear();
    mockFileHandler = std::make_shared<MockFileHandler>();
    DIContainer::registerInstance(mockFileHandler);

    // Resolve HardwareConfig using DIContainer
    hardwareConfig = DIContainer::resolve<HardwareConfig>();
  }

  void SetUp() override {
    // Setup code if needed
  }

  void TearDown() override {
    DIContainer::clear(); // Clear DIContainer after each test
  }
};

TEST_F(HardwareConfigTest, ConstructorInitialization) {
  // Assuming HardwareConfig has some method to verify its state
  // For example, checking if it's initialized with an empty configuration
  EXPECT_TRUE(hardwareConfig.getHardwarePinConfigs().empty());
}

TEST_F(HardwareConfigTest, LoadDeserializationFailure) {
  const std::string invalidMockFileContent = "This is not a JSON content";

  expectOpenReadCloseForContent(mockFileHandler, invalidMockFileContent,
                                DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::JsonInputInvalid)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(HardwareConfigTest, EmptyConfiguration) {
  const std::string emptyConfigJson = R"json({
    "components": {
      "singlePin": [],
      "multiPin": []
    }
  })json";

  expectOpenReadCloseForContent(mockFileHandler, emptyConfigJson,
                                DUMMY_FILE_PATH);
  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::OK);
  EXPECT_TRUE(hardwareConfig.getHardwarePinConfigs().empty());
}

TEST_F(HardwareConfigTest, InvalidConfigurationStructure) {
  const std::string invalidStructureJson = R"json({
    "invalidKey": {
      "someData": []
    }
  })json";

  expectOpenReadCloseForContent(mockFileHandler, invalidStructureJson,
                                DUMMY_FILE_PATH);
  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::HardwareConfigComponentsKeyMissing);
}

TEST_F(HardwareConfigTest, ParseSinglePinConfiguration) {
  const std::string validSinglePinJsonContent = R"json({
    "components": {
      "singlePin": [{
        "id": "TestPin",
        "type": "TestType",
        "pinNumber": 123,
        "options": {
          "option1": "value1",
          "option2": "value2"
        }
      }]
    }
  })json";

  expectOpenReadCloseForContent(mockFileHandler, validSinglePinJsonContent,
                                DUMMY_FILE_PATH);
                                
  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::OK);

  const auto &configs = hardwareConfig.getHardwarePinConfigs();

  ASSERT_EQ(configs.size(), 1);
  EXPECT_EQ(configs[0].id, "TestPin");
  EXPECT_EQ(configs[0].type, "TestType");
  EXPECT_EQ(configs[0].pinNumber, 123);
  EXPECT_EQ(configs[0].options.at("option1"), "value1");
  EXPECT_EQ(configs[0].options.at("option2"), "value2");
}

TEST_F(HardwareConfigTest, OptionParsing) {
  const std::string singlePinWithOptionsJson = R"json({
    "components": {
      "singlePin": [{
        "id": "TestPin",
        "type": "TestType",
        "pinNumber": 123,
        "options": {
          "option1": "value1",
          "option2": "value2"
        }
      }]
    }
  })json";

  expectOpenReadCloseForContent(mockFileHandler, singlePinWithOptionsJson,
                                DUMMY_FILE_PATH);
  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::OK);
  const auto &configs = hardwareConfig.getHardwarePinConfigs();
  ASSERT_EQ(configs.size(), 1);
  EXPECT_EQ(configs[0].options.at("option1"), "value1");
  EXPECT_EQ(configs[0].options.at("option2"), "value2");
}

TEST_F(HardwareConfigTest, CorrectPinConfigsRetrieval) {
  const std::string configJsonContent = R"json({
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

  expectOpenReadCloseForContent(mockFileHandler, configJsonContent,
                                DUMMY_FILE_PATH);
  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::OK);

  const auto &configs = hardwareConfig.getHardwarePinConfigs();
  ASSERT_EQ(configs.size(), 2);

  // Assertions for the single pin configuration
  const HardwarePinConfig &singlePinConfig = configs[0];
  EXPECT_EQ(singlePinConfig.id, "FlowMeterExtract");
  EXPECT_EQ(singlePinConfig.type, "FlowMeter");
  EXPECT_EQ(singlePinConfig.pinNumber, 15);
  EXPECT_EQ(singlePinConfig.options.at("pcntUnit"), "PCNT_UNIT_1");
  EXPECT_EQ(singlePinConfig.getOptionAs<int>("pulseMultiplier"), 100);
  EXPECT_EQ(singlePinConfig.getOptionAs<int>("filterValue"), 10);
  EXPECT_EQ(singlePinConfig.getOptionAs<bool>("filterEnabled"), true);

  // Assertions for the multi pin configuration
  const HardwarePinConfig &multiPinConfig = configs[1];
  EXPECT_EQ(multiPinConfig.id, "SPI");
  EXPECT_EQ(multiPinConfig.type, "SPI");
  EXPECT_EQ(multiPinConfig.pins.at("MOSI"), 23);
  EXPECT_EQ(multiPinConfig.pins.at("MISO"), 19);
  EXPECT_EQ(multiPinConfig.pins.at("SCLK"), 18);
  EXPECT_EQ(multiPinConfig.pins.at("CS"), 5);
}

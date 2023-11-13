#include "mocks/mock_FileHandler.h"
#include "mocks/mock_FileHandlerWithRealLoad.h"
#include "test_utils.h"
#include <HardwareConfig.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/************************** Test Summary ****************************/
/*

  Constructor Tests:
    - ConstructorInitialization: Verifies that the constructor correctly
  initializes the HardwareConfig object.

    Load Tests:
    - LoadSuccess: Tests successful loading of a valid configuration file and
  expects no errors.
    - LoadDeserializationFailure: Validates proper error handling when loading
  content that cannot be deserialized as JSON.

  save Method Tests:
    - SuccessfulSave: Tests the save function to ensure correct serialization
  and file writing.
    - SaveFileWriteFailure: Simulates a file write failure to test error
  handling in save method.

  getGpioConfigs Method Tests:
    - CorrectGpioConfigsRetrieval: Ensures that getGpioConfigs returns the
  correct gpioConfigs state.

  parseJson Method Tests:
    - MissingGpioPinsKey: Tests response to JSON missing the 'gpioPins' key.
    - GpioPinsNotArray: Checks the handling of 'gpioPins' key when it's not a
  JSON array.

  parseGpioPin Method Tests:
    - ValidGpioPinParsing: Verifies correct parsing of a valid GPIO pin JSON
  object.
    - HardwareConfigGpioPinNumberMissing: Tests for missing 'pinNumber' key in
  a GPIO pin configuration.
    - HardwareConfigGpioPinIdMissing: Tests for missing 'id' key in a GPIO pin
  configuration.
    - HardwareConfigGpioPinTypeMissing: Tests for missing 'type' key in a GPIO
  pin configuration.

  validateADCOptions Method Tests:
    - ValidAdcOptions: Ensures correct validation of valid ADC options.
    - MissingAdcOptionsKey: Tests for missing 'options' key in ADC
  configuration.
    - MissingADCResolution: Checks for missing or invalid 'resolution' key
  in ADC options.

*/
/*******************************************************************/

using ::testing::_;
using ::testing::Return;

class HardwareConfigTest : public ::testing::Test {
protected:
  HardwareConfigTest()
      : hardwareConfig(&mockFileHandler),
        realHardwareConfig(&mockFileHandlerWithRealLoad) {}

  const std::string DUMMY_FILE_PATH = "dummy_config.json";
  MockFileHandler mockFileHandler;
  HardwareConfig hardwareConfig;

  const std::string REAL_CONFIG_FILE = "test/test_config/hardware.json";
  MockFileHandlerWithRealLoad mockFileHandlerWithRealLoad;
  HardwareConfig realHardwareConfig;
};

TEST_F(HardwareConfigTest, LoadSuccess) {
  Error loadError = realHardwareConfig.load(REAL_CONFIG_FILE);
  EXPECT_EQ(loadError, Error::OK)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(HardwareConfigTest, LoadDeserializationFailure) {
  const std::string invalidMockFileContent = "This is not a JSON content";

  expectOpenReadCloseForContent(mockFileHandler, invalidMockFileContent,
                                DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::JsonInputInvalid)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(HardwareConfigTest, MissingGpioPinsKey) {
  // Prepare a mock JSON string without the 'gpioPins' key
  std::string invalidJson = R"({ "someOtherKey": [] })";

  // Mock the file handler to return this JSON string
  expectOpenReadCloseForContent(mockFileHandler, invalidJson, DUMMY_FILE_PATH);

  // Load the configuration
  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  // Expect the specific error for missing 'gpioPins' key
  EXPECT_EQ(loadError, Error::HardwareConfigGpioPinsKeyMissing)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(HardwareConfigTest, SuccessfulSave) {
  std::string expectedJson = R"({
    "gpioPins": [
      {"pinNumber": 1, "id": "pin1", "type": "Digital"}
    ]
  })";

  EXPECT_CALL(mockFileHandler, open(_, "w")).WillOnce(Return(true));
  EXPECT_CALL(mockFileHandler, write(testing::Ne(""))).WillOnce(Return(true));
  EXPECT_CALL(mockFileHandler, close()).Times(1);

  Error saveError = hardwareConfig.save("test_config.json");
  EXPECT_EQ(saveError, Error::OK);
}

TEST_F(HardwareConfigTest, SaveFileWriteFailure) {
  EXPECT_CALL(mockFileHandler, open(_, "w")).WillOnce(Return(true));
  EXPECT_CALL(mockFileHandler, write(testing::_))
      .WillOnce(Return(false)); // Use testing::_ for any argument
  EXPECT_CALL(mockFileHandler, close()).Times(1);

  Error saveError = hardwareConfig.save("test_config.json");
  EXPECT_EQ(saveError, Error::FileWriteFailure);
}

TEST_F(HardwareConfigTest, CorrectGpioConfigsRetrieval) {
  std::string validJson = R"({
    "gpioPins": [
      {"pinNumber": 3, "id": "pin3", "type": "PWM"}
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, validJson, DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);
  ASSERT_EQ(loadError, Error::OK)
      << loadError.getFormattedMessage(loadError.code());
  ;

  const auto &gpioConfigs = hardwareConfig.getGpioConfigs();
  ASSERT_EQ(gpioConfigs.size(), 1u);
  EXPECT_EQ(gpioConfigs[0].pinNumber, 3)<< loadError.getFormattedMessage(loadError.code());;
  EXPECT_EQ(gpioConfigs[0].id, "pin3");
  EXPECT_EQ(gpioConfigs[0].type, "PWM");
}

TEST_F(HardwareConfigTest, GpioPinsNotArray) {
  std::string invalidJson = R"({
    "gpioPins": {"pinNumber": 1, "id": "pin1", "type": "Digital"}
  })";

  expectOpenReadCloseForContent(mockFileHandler, invalidJson, DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);

  EXPECT_EQ(loadError, Error::HardwareConfigGpioPinsTypeMismatch)
      << loadError.getFormattedMessage(loadError.code());
}

TEST_F(HardwareConfigTest, ValidGpioPinParsing) {
  std::string validJson = R"({
    "gpioPins": [
      {"pinNumber": 4, "id": "pin4", "type": "Digital"}
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, validJson, DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);
  ASSERT_EQ(loadError, Error::OK)
      << loadError.getFormattedMessage(loadError.code());
  ;

  const auto &gpioConfigs = hardwareConfig.getGpioConfigs();
  ASSERT_EQ(gpioConfigs.size(), 1u);
  EXPECT_EQ(gpioConfigs[0].pinNumber, 4);
  EXPECT_EQ(gpioConfigs[0].id, "pin4");
  EXPECT_EQ(gpioConfigs[0].type, "Digital");
}

TEST_F(HardwareConfigTest, HardwareConfigGpioPinNumberMissing) {
  std::string invalidJson = R"({
    "gpioPins": [
      {"id": "pin10", "type": "Digital"}
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, invalidJson, DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);
  EXPECT_EQ(loadError, Error::HardwareConfigGpioPinNumberMissing)
      << loadError.getFormattedMessage(loadError.code());
  ;
}

TEST_F(HardwareConfigTest, HardwareConfigGpioPinIdMissing) {
  std::string invalidJson = R"({
    "gpioPins": [
      {"pinNumber": 11, "type": "Digital"}
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, invalidJson, DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);
  EXPECT_EQ(loadError, Error::HardwareConfigGpioPinIdMissing)
      << loadError.getFormattedMessage(loadError.code());
  ;
}

TEST_F(HardwareConfigTest, HardwareConfigGpioPinTypeMissing) {
  std::string invalidJson = R"({
    "gpioPins": [
      {"pinNumber": 12, "id": "pin12"}
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, invalidJson, DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);
  EXPECT_EQ(loadError, Error::HardwareConfigGpioPinTypeMissing)
      << loadError.getFormattedMessage(loadError.code());
  ;
}

TEST_F(HardwareConfigTest, MissingAdcOptionsKey) {
  std::string invalidJson = R"({
    "gpioPins": [
      {"pinNumber": 8, "id": "pin8", "type": "ADC"}
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, invalidJson, DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);
  EXPECT_EQ(loadError, Error::HardwareConfigAdcOptionsKeyMissing)
      << loadError.getFormattedMessage(loadError.code());
  ;
}

TEST_F(HardwareConfigTest, MissingADCResolution) {
  std::string invalidJson = R"({
    "gpioPins": [
      {"pinNumber": 9, "id": "pin9", "type": "ADC", "options": {"resolution": "high"}}
    ]
  })";

  expectOpenReadCloseForContent(mockFileHandler, invalidJson, DUMMY_FILE_PATH);

  Error loadError = hardwareConfig.load(DUMMY_FILE_PATH);
  EXPECT_EQ(loadError, Error::HardwareConfigAdcResolutionOptionMissing)
      << loadError.getFormattedMessage(loadError.code());
  ;
}

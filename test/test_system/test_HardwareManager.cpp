#include "ConfigManager.h"
#include "GpioPinConfig.h"
#include "HardwareFactory.h"
#include "HardwareManager.h"
#include "IFileHandler.h"
#include "Logger.h"
#include "MockButtonController.h"
#include "MockConfigManager.h"
#include "MockHardwareConfig.h"
#include "MockHardwareFactory.h"
#include "Mocks/MockFileHandler.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/************************** Test Summary ****************************/
/*

  HardwareManager Initialization Tests:

  - InitializeWithValidConfig: Validates that the HardwareManager correctly
  initializes hardware components with a valid configuration.
  - InitializeWithInvalidConfig: Tests the HardwareManager's response to an
  invalid or malformed hardware configuration.
  - InitializeWithoutConfig: Checks how the HardwareManager behaves when no
  configuration is available.
  - SensorInitializationSuccess: Ensures proper initialization of sensors with
  correct configuration settings.
  - SensorInitializationFailure: Tests HardwareManager's handling of
  unsuccessful sensor initialization.
  - MotorInitializationSuccess: Verifies correct initialization of motors with
  valid configuration.
  - MotorInitializationFailure: Tests response to failed motor initialization
  due to issues like motor driver problems.
  - ErrorHandlingOnInitialization: Assesses the error handling during the
  initialization process.
  - EventNotificationPostInitialization: Checks if the HardwareManager notifies
  other components after successful initialization.
  - StateTransitionAfterInitialization: Evaluates state transitions using
  TinyFSM following the initialization process.
  - ResourceReleaseOnFailure: Tests resource cleanup and release in case of
  initialization failure.
  - LoggingDuringInitialization: Verifies logging of steps and issues during the
  initialization phase.

*/
/*******************************************************************/

using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;

class HardwareManagerTest : public ::testing::Test {
protected:
  std::shared_ptr<MockFileHandler> mockFileHandler;
  std::shared_ptr<MockConfigManager> mockConfigManager;
  std::shared_ptr<MockHardwareConfig> mockHardwareConfig;
  std::unique_ptr<MockHardwareFactory> mockHardwareFactory;
  std::shared_ptr<MockButtonController> mockButtonController;

  HardwareManager hardwareManager;

  HardwareManagerTest()
      : mockFileHandler(new MockFileHandler()),
        mockConfigManager(new MockConfigManager(mockFileHandler.get())),
        mockHardwareConfig(new MockHardwareConfig(mockFileHandler.get())),
        mockHardwareFactory(new MockHardwareFactory()),
        hardwareManager(mockConfigManager, std::move(mockHardwareFactory),
                        mockButtonController) {}

  std::ostringstream capturedLog;

  void SetUp() override {
    Logger::setLogCallback(
        [this](Logger::Level level, const std::string &message) {
          capturedLog << Logger::levelToString(level) << ": " << message
                      << std::endl;
        });
  }

  void TearDown() override {
    Logger::setLogCallback(nullptr); // Reset the logger callback
  }
};

TEST_F(HardwareManagerTest, InitializeWithValidConfig) {
  // Set up mock config and expectations
  GpioPinConfig pin1{1, "Pin1", "DigitalIO"};
  GpioPinConfig pin2{2, "Pin2", "PWM"};
  GpioPinConfig pin3{3, "Pin3", "DigitalIO"};
  pin3.options["mode"] = "INPUT";

  std::vector<GpioPinConfig> gpioConfigurations = {pin1, pin2, pin3};

  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));

  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillOnce(ReturnRef(gpioConfigurations));

  // Call initializeHardware()
  hardwareManager.initializeHardware();

  // Verify components initialized
  EXPECT_TRUE(hardwareManager.isComponentInitialized("Pin1"));
  EXPECT_TRUE(hardwareManager.isComponentInitialized("Pin2"));
  EXPECT_TRUE(hardwareManager.isComponentInitialized("Pin3"));
}

TEST_F(HardwareManagerTest, InitializeWithoutConfig) {
  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(nullptr));

  hardwareManager.initializeHardware();
  
  EXPECT_FALSE(hardwareManager.isComponentInitialized("Button1"));
  EXPECT_FALSE(hardwareManager.isComponentInitialized("PWM1"));
}

TEST_F(HardwareManagerTest, InitializeADCWithValidConfig) {
  GpioPinConfig adcConfig{13, "VoltageSense", "ADC"};
  adcConfig.options["resolution"] = "12";
  adcConfig.options["attenuation"] = "3";

  std::vector<GpioPinConfig> gpioConfigurations = {adcConfig};

  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));
  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillOnce(ReturnRef(gpioConfigurations));

  hardwareManager.initializeHardware();

  EXPECT_TRUE(hardwareManager.isComponentInitialized("VoltageSense"));
}

TEST_F(HardwareManagerTest, InitializeDigitalIOWithValidConfig) {
  GpioPinConfig digitalIOConfig{26, "MotorControlFill", "DigitalIO"};
  digitalIOConfig.options["mode"] = "OUTPUT";

  std::vector<GpioPinConfig> gpioConfigurations = {digitalIOConfig};

  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));
  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillOnce(ReturnRef(gpioConfigurations));

  hardwareManager.initializeHardware();

  EXPECT_TRUE(hardwareManager.isComponentInitialized("MotorControlFill"));
}

TEST_F(HardwareManagerTest, InitializePWMWithValidConfig) {
  GpioPinConfig pwmConfig{25, "MotorSpeed", "PWM"};
  std::vector<GpioPinConfig> gpioConfigurations = {pwmConfig};

  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));
  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillOnce(ReturnRef(gpioConfigurations));

  hardwareManager.initializeHardware();

  EXPECT_TRUE(hardwareManager.isComponentInitialized("MotorSpeed"));
}

TEST_F(HardwareManagerTest, InitializeBuzzerWithValidConfig) {
  GpioPinConfig buzzerConfig{12, "Buzzer", "Buzzer"};
  std::vector<GpioPinConfig> gpioConfigurations = {buzzerConfig};

  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));
  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillOnce(ReturnRef(gpioConfigurations));

  hardwareManager.initializeHardware();

  EXPECT_TRUE(hardwareManager.isComponentInitialized("Buzzer"));
}

TEST_F(HardwareManagerTest, TestInitializationOfNonExistentHardwareType) {
  // Set up mock config with a non-existent hardware type
  GpioPinConfig nonExistentTypeConfig{5, "NonExistentID",
                                      "NonExistentType"};

  std::vector<GpioPinConfig> gpioConfigurations = {nonExistentTypeConfig};

  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));

  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillOnce(ReturnRef(gpioConfigurations));

  // Call initializeHardware()
  hardwareManager.initializeHardware();

  // Verify that the hardware component of non-existent type is not initialized
  // Assuming that "NonExistentType" is not a recognized type in
  // initializeComponent method
  EXPECT_FALSE(hardwareManager.isComponentInitialized("NonExistentID"));
}

TEST_F(HardwareManagerTest, TestErrorHandlingOnFailedInitialization) {
  GpioPinConfig invalidConfig{1, "FailedPin", "DigitalIO"};
  invalidConfig.options["mode"] =
      "INVALID_MODE"; // Intentionally invalid to cause failure
  std::vector<GpioPinConfig> gpioConfigurations = {invalidConfig};

  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));
  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillOnce(ReturnRef(gpioConfigurations));

  hardwareManager.initializeHardware();

  // Verify if the DigitalIOModeOptionMissingOrInvalid error is generated
  // Assuming Error class has a static method to check the last error generated
  EXPECT_EQ(Error::getLastErrorCode(),
            Error::DigitalIOModeOptionMissingOrInvalid);
}

TEST_F(HardwareManagerTest, TestLoggingOfSuccessfulInitialization) {
  // Set up mock config and expectations
  GpioPinConfig pin1{1, "Pin1", "DigitalIO"};
  GpioPinConfig pin2{2, "Pin2", "PWM"};
  GpioPinConfig pin3{3, "Pin3", "DigitalIO"};
  pin3.options["mode"] = "INPUT";

  std::vector<GpioPinConfig> gpioConfigurations = {pin1, pin2, pin3};

  // Mock dependencies to simulate successful initialization
  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));
  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillRepeatedly(
          ReturnRef(gpioConfigurations)); // Provide valid configurations

  // Act
  hardwareManager.initializeHardware();

  // Assert
  std::string logContents = capturedLog.str();
  EXPECT_THAT(logContents,
              testing::HasSubstr("Hardware initialization successful"));
}

TEST_F(HardwareManagerTest, TestLoggingOfConfigurationIssues) {
  // Mock dependencies to simulate a configuration issue
  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(nullptr)); // Simulate missing config

  // Act
  hardwareManager.initializeHardware();

  // Assert
  std::string logContents = capturedLog.str();
  EXPECT_THAT(
      logContents,
      testing::HasSubstr("ERROR: Hardware configuration is not available\n"));
}

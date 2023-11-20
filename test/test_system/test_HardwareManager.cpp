#include "ConfigManager.h"
#include "GpioPinConfig.h"
#include "HardwareFactory.h"
#include "HardwareManager.h"
#include "IFileHandler.h"
#include "MockConfigManager.h"
#include "MockFileHandler.h"
#include "MockHardwareConfig.h"
#include "MockHardwareFactory.h"
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


  1. Successful Initialization
Test Case: Ensure that HardwareManager correctly initializes hardware components
when ConfigManager provides valid HardwareConfig data. Expectation: The hardware
components are initialized as expected, and any relevant state in
HardwareManager is set correctly.
2. Handling of Missing or Invalid Configurations
Test Case: Simulate a scenario where ConfigManager returns nullptr (indicating
missing configuration) and verify how HardwareManager handles it. Expectation:
HardwareManager should not attempt to initialize hardware components and should
handle the situation gracefully, possibly setting an error state or logging an
appropriate message.
3. Error Conditions and Robustness
Test Case: Introduce mock errors or exceptions in the configuration loading
process and observe how HardwareManager responds. Expectation: The manager
should handle errors robustly, without crashing or entering an invalid state.
4. Correct Interaction with ConfigManager
Test Case: Verify that HardwareManager calls the correct methods on
ConfigManager to retrieve the necessary configurations. Expectation: Methods
such as getHardwareConfig() are called as expected.
5. Handling Partial or Incomplete Configurations
Test Case: Provide a HardwareConfig with partial or incomplete data (e.g.,
missing some GPIO configurations) and test how HardwareManager responds.
Expectation: HardwareManager should either handle the incomplete configuration
gracefully or log appropriate errors/warnings.
6. State Verification Post Initialization
Test Case: After successful initialization, verify that the state of
HardwareManager reflects the initialized hardware. Expectation: Internal state
variables or flags in HardwareManager should correctly represent the
initialization status.
7. Multiple Initializations
Test Case: Call the initialization method on HardwareManager multiple times,
possibly with different configurations. Expectation: Subsequent initializations
either update the hardware state correctly or are handled appropriately (e.g.,
ignored, logged, etc.).

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

  HardwareManager hardwareManager;

  HardwareManagerTest()
      : mockFileHandler(new MockFileHandler()),
        mockConfigManager(new MockConfigManager(mockFileHandler.get())),
        mockHardwareConfig(new MockHardwareConfig(mockFileHandler.get())),
        mockHardwareFactory(new MockHardwareFactory()),
        hardwareManager(mockConfigManager, std::move(mockHardwareFactory)) {}

  virtual void SetUp() override {
    // Setup code (if needed)
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
  EXPECT_TRUE(hardwareManager.isComponentInitialized("DigitalIO"));
  EXPECT_TRUE(hardwareManager.isComponentInitialized("PWM"));
}

TEST_F(HardwareManagerTest, InitializeWithoutConfig) {
  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(nullptr));

  hardwareManager.initializeHardware();

  EXPECT_FALSE(hardwareManager.isComponentInitialized("DigitalIO"));
  EXPECT_FALSE(hardwareManager.isComponentInitialized("PWM"));
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

  EXPECT_TRUE(hardwareManager.isComponentInitialized("ADC"));
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

  EXPECT_TRUE(hardwareManager.isComponentInitialized("DigitalIO"));
}

TEST_F(HardwareManagerTest, InitializePWMWithValidConfig) {
  GpioPinConfig pwmConfig{25, "MotorSpeed", "PWM"};
  std::vector<GpioPinConfig> gpioConfigurations = {pwmConfig};

  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));
  EXPECT_CALL(*mockHardwareConfig, getGpioConfigs())
      .WillOnce(ReturnRef(gpioConfigurations));

  hardwareManager.initializeHardware();

  EXPECT_TRUE(hardwareManager.isComponentInitialized("PWM"));
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
  GpioPinConfig nonExistentTypeConfig{5, "NonExistentTypePin",
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
  EXPECT_FALSE(hardwareManager.isComponentInitialized("NonExistentType"));
}

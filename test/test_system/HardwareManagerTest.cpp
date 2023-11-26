#include "HardwareManager.h"
#include "ConfigManager.h"
#include "HardwareFactory.h"
#include "HardwarePinConfig.h"
#include "IFileHandler.h"
#include "Logger.h"
#include "MockButtonController.h"
#include "MockConfigManager.h"
#include "MockHardwareConfig.h"
#include "MockHardwareFactory.h"
#include "Mocks/MockFileHandler.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

/************************** Test Summary ****************************

Constructor Tests:

InitializeHardwareSuccess: Tests the initializeHardware method to verify that
all hardware components are successfully initialized when valid hardware
configuration is provided. InitializeHardwareConfigNotFound: Checks the behavior
when no hardware configuration is found, expecting an appropriate log message
and no initialization. InitializeHardwarePartialFailure: Simulates partial
failure in hardware initialization (some components fail to initialize) and
verifies the appropriate handling and logging. Component Initialization Tests:

InitializeComponentSuccess: Verifies that initializeComponent successfully
initializes a given hardware component, adds it to the components map, and logs
the creation. InitializeComponentFailure: Tests the failure scenario in
component creation, ensuring correct error logging and return value. Register
Component Tests:

RegisterComponentButton: Checks if a button component is correctly registered
with the ButtonController. RegisterComponentTypeMismatch: Tests the behavior
when attempting to register a component with a type mismatch (e.g., non-button
component as a button). Component Retrieval Tests:

GetComponentByIdExisting: Verifies that getComponentById successfully retrieves
an existing component. GetComponentByIdNonExisting: Tests the behavior when
attempting to retrieve a non-existing component, expecting a nullptr. Component
Initialization Check Tests:

IsComponentInitializedTrue: Verifies that isComponentInitialized returns true
for an initialized component. IsComponentInitializedFalse: Checks if
isComponentInitialized returns false for non-initialized or non-existing
components. Update Method Tests:

UpdateMethodCall: Tests that the update method correctly logs an update
notification. Button Event Handling Tests:

OnButtonPressEventHandled: Verifies that onButtonEvent correctly handles a
button press event. OnButtonEventIgnoreUnpressed: Checks if onButtonEvent
ignores events where the button is not pressed. State Change Tests:

ChangeStateBasedOnButton: Ensures that changeStateBasedOnButton correctly
triggers state changes in the state machine based on button ID. Buzzer Trigger
Tests:

TriggerBuzzerSuccess: Tests that triggerBuzzer successfully triggers the buzzer
component when it is available and correctly typed. TriggerBuzzerNotFound:
Verifies the behavior when the buzzer component is not found in the components
map. TriggerBuzzerTypeMismatch: Checks the behavior when a non-buzzer component
is mistakenly identified as a buzzer.
********************************************************************/

using ::testing::_;
using ::testing::DoAll;
using ::testing::HasSubstr;
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

  HardwarePinConfig adcConfig{1, "VoltageSense", "ADC"};
  HardwarePinConfig buttonConfig{22, "ButtonStart", "Button"};
  HardwarePinConfig digitalIOConfig{23, "LPS", "DigitalIO"};
  std::vector<HardwarePinConfig> mockConfigs = {adcConfig, buttonConfig,
                                                digitalIOConfig};

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

TEST_F(HardwareManagerTest, CheckConfigBeforeInitialization) {
  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(nullptr));

  hardwareManager.initializeHardware();

  EXPECT_THAT(capturedLog.str(),
              HasSubstr("Hardware configuration is not available"));
}

TEST_F(HardwareManagerTest, InitializeHardwareSuccess) {
  // Setup MockHardwareConfig to return the mock configurations
  ON_CALL(*mockHardwareConfig, getHardwarePinConfigs())
      .WillByDefault(ReturnRef(mockConfigs));

  // Setup MockConfigManager to return the mock hardware configuration
  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(mockHardwareConfig));

  // Configure the mock hardware factory to use createComponent
  // ON_CALL(*mockHardwareFactory, createComponent(_))
  //     .WillByDefault(Invoke([this](const HardwarePinConfig &config)
  //                               -> std::unique_ptr<HardwareComponent> {
  //       if (config.type == "ADC") {
  //         return std::unique_ptr<MockADC>(new MockADC(config));
  //       }
  //       // Add other conditions for different hardware types
  //       return nullptr;
  //     }));

  // Call initializeHardware
  hardwareManager.initializeHardware();

  // Check that all components in mockConfigs are initialized
  for (const auto &config : mockConfigs) {
    EXPECT_TRUE(hardwareManager.isComponentInitialized(config.id));
  }

  // Check the log for a successful initialization message
  EXPECT_THAT(capturedLog.str(),
              HasSubstr("Hardware initialization successful"));
}

TEST_F(HardwareManagerTest, InitializeHardwareConfigNotFound) {
  EXPECT_CALL(*mockConfigManager, getHardwareConfig())
      .WillOnce(Return(nullptr)); // Simulate no hardware config available

  hardwareManager.initializeHardware();

  EXPECT_THAT(capturedLog.str(),
              testing::HasSubstr("Hardware configuration is not available"));
}

// TEST_F(HardwareManagerTest, TestLoggingOfSuccessfulInitialization) {
//   // Set up mock config and expectations
//   HardwarePinConfig pin1{1, "Pin1", "DigitalIO"};
//   HardwarePinConfig pin2{2, "Pin2", "PWM"};
//   HardwarePinConfig pin3{3, "Pin3", "DigitalIO"};
//   pin3.options["mode"] = "INPUT";

//   std::vector<HardwarePinConfig> gpioConfigurations = {pin1, pin2, pin3};

//   // Mock dependencies to simulate successful initialization
//   EXPECT_CALL(*mockConfigManager, getHardwareConfig())
//       .WillOnce(Return(mockHardwareConfig));
//   EXPECT_CALL(*mockHardwareConfig, getHardwarePinConfigs())
//       .WillRepeatedly(
//           ReturnRef(gpioConfigurations)); // Provide valid configurations

//   // Act
//   hardwareManager.initializeHardware();

//   // Assert
//   std::string logContents = capturedLog.str();
//   EXPECT_THAT(logContents,
//               testing::HasSubstr("Hardware initialization successful"));
// }

// TEST_F(HardwareManagerTest, TestLoggingOfConfigurationIssues) {
//   // Mock dependencies to simulate a configuration issue
//   EXPECT_CALL(*mockConfigManager, getHardwareConfig())
//       .WillOnce(Return(nullptr)); // Simulate missing config

//   // Act
//   hardwareManager.initializeHardware();

//   // Assert
//   std::string logContents = capturedLog.str();
//   EXPECT_THAT(
//       logContents,
//       testing::HasSubstr("ERROR: Hardware configuration is not
//       available\n"));
// }

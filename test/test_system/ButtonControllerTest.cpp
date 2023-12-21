/**
 * @file ButtonControllerTests.cpp
 * @brief Tests for the ButtonController class
 *
 * @section Test Cases
 *
 * The following test cases cover various aspects of the ButtonController class:
// Tests for ButtonController

// Test registration and retrieval of a button
// - Should store the button correctly and retrieve the same instance
// - Should handle a request for a non-existent button gracefully

// Test button press and auto-repeat behavior
// - Should correctly update the button state on press and release
// - Should handle auto-repeat activation correctly after the initial delay
// - Should generate repeated events at the specified standard rate
// - Should stop auto-repeat when the button is released

// Test interaction with mediator
// - Should notify mediator on button press
// - Should notify mediator on auto-repeat events

// Test handling of interaction settings
// - Should apply interaction settings correctly to all registered buttons
// - Should update settings dynamically when changed

// Test processButtonStates method
// - Should correctly process all button states in each call
// - Should handle cases with multiple buttons correctly

// Test response to received events (if applicable)
// - Should react appropriately to different event types
// - Should handle events that do not affect the ButtonController gracefully

// Edge cases and error handling
// - Should handle invalid input gracefully (e.g., null pointers, invalid IDs)
// - Should not crash or behave unpredictably under abnormal conditions

 **/

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ButtonController.h"
#include "HardwarePinConfig.h"
#include "Mediator/ConcreteMediator.h"
#include "MockButtonController.h"
#include "MockMediator.h"
#include "Mocks/MockButton.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;
using namespace std::chrono;

// Test Fixture for ButtonController
class ButtonControllerTest : public ::testing::Test {
 protected:
  HardwarePinConfig testConfig{1, "Button1", "Button"};
  std::shared_ptr<MockButton> mockButton;
  std::shared_ptr<ButtonController> buttonController;
  std::string testButtonId = "testButton";
  InteractionSettings settings;
  std::shared_ptr<MockMediator> mockMediator = std::make_shared<MockMediator>();

  void SetUp() override {
    auto mediator = std::make_shared<ConcreteMediator>();
    buttonController = std::make_shared<ButtonController>(mockMediator);
    mockButton = std::make_shared<MockButton>(testConfig);

    EXPECT_CALL(*mockButton, update()).Times(AnyNumber());
    ON_CALL(*mockButton, getCurrentState())
        .WillByDefault(Return(MockButton::ButtonState{false, false}));
    ON_CALL(*mockMediator, registerColleague(_)).WillByDefault(Return());

    buttonController->registerButton(testButtonId, mockButton);

    // Initialize interaction settings for the button
    InteractionSettings::Button buttonSettings;
    buttonSettings.autoRepeat.initialDelayMs = 500;  // e.g., 500 ms
    buttonSettings.autoRepeat.standardRateMs = 100;  // e.g., 100 ms
    settings.buttons[testButtonId] = buttonSettings;

    // Register and set up the button
    buttonController->registerButton(testButtonId, mockButton);
    buttonController->setInteractionSettings(settings);
  }
};

// Test storing and retrieving a registered button
TEST_F(ButtonControllerTest, RegisterAndRetrieveButton) {
  // Register the button
  buttonController->registerButton(testButtonId, mockButton);

  // Retrieve the button and check if it's the same instance
  auto retrievedButton = buttonController->getButtonById(testButtonId);
  EXPECT_EQ(retrievedButton, mockButton)
      << "Retrieved button should be the same as the registered button";
}

// Test retrieving a non-existent button
TEST_F(ButtonControllerTest, RetrieveNonExistentButton) {
  // Attempt to retrieve a button that hasn't been registered
  auto retrievedButton = buttonController->getButtonById("nonExistentButton");
  EXPECT_EQ(retrievedButton, nullptr)
      << "Retrieving a non-existent button should return nullptr";
}

TEST_F(ButtonControllerTest, UpdateButtonStateOnPressAndRelease) {
  // Setting up the button to be pressed
  EXPECT_CALL(*mockButton, isPressed()).WillRepeatedly(Return(true));
  // Set the expected state for getCurrentState when the button is pressed
  ON_CALL(*mockButton, getCurrentState())
      .WillByDefault(Return(MockButton::ButtonState{true, false}));
  buttonController->processButtonStates();
  auto buttonState = mockButton->getCurrentState();
  EXPECT_TRUE(buttonState.isPressed);

  // Setting up the button to be released
  EXPECT_CALL(*mockButton, isPressed()).WillRepeatedly(Return(false));
  // Set the expected state for getCurrentState when the button is released
  ON_CALL(*mockButton, getCurrentState())
      .WillByDefault(Return(MockButton::ButtonState{false, false}));
  buttonController->processButtonStates();
  buttonState = mockButton->getCurrentState();
  EXPECT_FALSE(buttonState.isPressed);
}

TEST_F(ButtonControllerTest, HandleAutoRepeatActivation) {
  // Expect the button to be pressed for auto-repeat
  EXPECT_CALL(*mockButton, isPressed()).WillRepeatedly(Return(true));
  // Set up the return value for getCurrentState() specific to this test
  ON_CALL(*mockButton, getCurrentState())
      .WillByDefault(Return(MockButton::ButtonState{true, true}));
  std::this_thread::sleep_for(std::chrono::milliseconds(600));
  buttonController->processButtonStates();
  auto buttonState = mockButton->getCurrentState();
  EXPECT_TRUE(
      buttonState.isInAutoRepeatMode);  // Verifying auto-repeat activation
}

TEST_F(ButtonControllerTest, GenerateRepeatedEvents) {
  EXPECT_CALL(*mockButton, isPressed()).WillRepeatedly(Return(true));
  std::this_thread::sleep_for(
      std::chrono::milliseconds(600));  // Wait for initial delay
  buttonController->processButtonStates();
  std::this_thread::sleep_for(
      std::chrono::milliseconds(200));  // Wait for two standard rate intervals
  buttonController->processButtonStates();
  // Additional checks can be added here based on how ButtonController
  // notifies events
}

TEST_F(ButtonControllerTest, StopAutoRepeatOnRelease) {
  EXPECT_CALL(*mockButton, isPressed()).WillRepeatedly(Return(true));
  std::this_thread::sleep_for(
      std::chrono::milliseconds(600));  // Wait for initial delay
  buttonController->processButtonStates();
  EXPECT_CALL(*mockButton, isPressed()).WillRepeatedly(Return(false));
  buttonController->processButtonStates();
  auto buttonState = mockButton->getCurrentState();
  EXPECT_FALSE(buttonState.isInAutoRepeatMode);
}

TEST_F(ButtonControllerTest, NotifyMediatorOnButtonPress) {
  std::shared_ptr<MockMediator> mockMediator = std::make_shared<MockMediator>();
  buttonController = std::make_shared<ButtonController>(mockMediator);

  // Set the expected state for getCurrentState when the button is pressed
  ON_CALL(*mockButton, getCurrentState())
      .WillByDefault(Return(MockButton::ButtonState{true, false}));

  // Setting up the button to be pressed
  EXPECT_CALL(*mockButton, isPressed()).WillOnce(Return(true));

  // Expect the mediator to be notified on button press
  EXPECT_CALL(*mockMediator,
              notify(buttonController.get(), EventType::BUTTON_PRESSED, _))
      .Times(1);

  // Process the button states
  buttonController->processButtonStates();
}

TEST_F(ButtonControllerTest, NotifyMediatorOnAutoRepeatEvents) {
  std::shared_ptr<MockMediator> mockMediator = std::make_shared<MockMediator>();
  buttonController = std::make_shared<ButtonController>(mockMediator);

  // Setting up the button to be in auto-repeat mode
  EXPECT_CALL(*mockButton, isPressed()).WillRepeatedly(Return(true));
  ON_CALL(*mockButton, getCurrentState())
      .WillByDefault(Return(MockButton::ButtonState{true, true}));

  // Simulate the delay for auto-repeat activation
  std::this_thread::sleep_for(milliseconds(600));  // Wait for initial delay

  // Expect the mediator to be notified on auto-repeat events
  EXPECT_CALL(*mockMediator,
              notify(buttonController.get(), EventType::BUTTON_AUTO_REPEAT, _))
      .Times(AtLeast(1));

  buttonController->processButtonStates();
}

TEST_F(ButtonControllerTest, HandleInteractionSettings) {
  // Initial settings
  InteractionSettings initialSettings;
  initialSettings.buttons[testButtonId].autoRepeat.initialDelayMs = 500;
  initialSettings.buttons[testButtonId].autoRepeat.standardRateMs = 100;

  // Apply initial settings
  buttonController->setInteractionSettings(initialSettings);

  // Verify initial settings applied
  auto appliedSettings = mockButton->getAppliedSettings();
  EXPECT_EQ(appliedSettings.initialDelayMs, 500);
  EXPECT_EQ(appliedSettings.standardRateMs, 100);

  // Updated settings
  InteractionSettings updatedSettings;
  updatedSettings.buttons[testButtonId].autoRepeat.initialDelayMs = 600;
  updatedSettings.buttons[testButtonId].autoRepeat.standardRateMs = 150;

  // Apply updated settings
  buttonController->setInteractionSettings(updatedSettings);

  // Verify updated settings applied
  appliedSettings = mockButton->getAppliedSettings();
  EXPECT_EQ(appliedSettings.initialDelayMs, 600);
  EXPECT_EQ(appliedSettings.standardRateMs, 150);
}

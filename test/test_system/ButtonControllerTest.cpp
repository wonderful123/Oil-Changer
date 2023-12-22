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
  InteractionSettings interactionSettings;
  std::shared_ptr<MockMediator> mockMediator = std::make_shared<MockMediator>();

  void SetUp() override {
    buttonController = std::make_shared<ButtonController>(mockMediator);
    mockButton = std::make_shared<MockButton>(testConfig);

    ON_CALL(*mockMediator, registerColleague(_)).WillByDefault(Return());

    buttonController->registerButton(testButtonId, mockButton);

    // Initialize interaction settings for the button
    interactionSettings.buttons[testButtonId].autoRepeat.initialDelayMs = 500;
    interactionSettings.buttons[testButtonId].autoRepeat.standardRateMs = 100;
    buttonController->setInteractionSettings(interactionSettings);

    auto appliedSettings = mockButton->getAppliedSettings();
    EXPECT_EQ(appliedSettings.initialDelayMs, 500);
    EXPECT_EQ(appliedSettings.standardRateMs, 100);
  }

  void SimulateTimePassage(int milliseconds) {
    auto duration = std::chrono::milliseconds(milliseconds);
    // Set the mock button's internal state to simulate time passage
    mockButton->mockAdjustLastPressTime(duration);
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
};

// Test retrieving a non-existent button
TEST_F(ButtonControllerTest, RetrieveNonExistentButton) {
  // Attempt to retrieve a button that hasn't been registered
  auto retrievedButton = buttonController->getButtonById("nonExistentButton");
  EXPECT_EQ(retrievedButton, nullptr)
      << "Retrieving a non-existent button should return nullptr";
};

TEST_F(ButtonControllerTest, UpdateButtonStateOnPressAndRelease) {
  // Setting up the button to be pressed
  mockButton->simulatePress(true);
  buttonController->processButtonStates();
  auto buttonState = mockButton->getCurrentState();
  EXPECT_TRUE(buttonState.isPressed);

  // Simulate button press and update the button state
  mockButton->simulatePress(false);
  buttonController->processButtonStates();

  // Verify that the button state reflects 'pressed'
  buttonState = mockButton->getCurrentState();
  EXPECT_FALSE(buttonState.isPressed);
};

TEST_F(ButtonControllerTest, HoldingButtonChangesStateToAutoRepeat) {
  auto buttonState = mockButton->getCurrentState();
  EXPECT_FALSE(buttonState.isPressed);
  
  // Set the button to be continuously pressed
  mockButton->simulatePress(true);
  buttonController->processButtonStates();
  buttonState = mockButton->getCurrentState();
  EXPECT_TRUE(buttonState.isPressed);
  EXPECT_FALSE(buttonState.isInAutoRepeatMode);

  // Simulate initial delay for auto-repeat activation
  SimulateTimePassage(5001);  // Wait for initial delay
  buttonController->processButtonStates();
  buttonState = mockButton->getCurrentState();
  EXPECT_TRUE(buttonState.isInAutoRepeatMode);

  // Simulate additional time for a couple of auto-repeat intervals
  // SimulateTimePassage(200);  // Wait for two standard rate intervals
  // buttonController->processButtonStates();
  // buttonState = mockButton->getCurrentState();
  // EXPECT_TRUE(buttonState.isInAutoRepeatMode);
};

TEST_F(ButtonControllerTest, StopAutoRepeatOnRelease) {
  // Simulate button press and initial delay for auto-repeat activation
  mockButton->simulatePress(true);
  buttonController->processButtonStates();
  auto buttonState = mockButton->getCurrentState();
  EXPECT_TRUE(buttonState.isPressed);

  SimulateTimePassage(501);  // Wait for initial delay
  buttonController->processButtonStates();
  buttonState = mockButton->getCurrentState();
  EXPECT_TRUE(buttonState.isInAutoRepeatMode);

  // Simulate button release
  mockButton->simulatePress(false);
  buttonController->processButtonStates();

  // Check if the button exits auto-repeat mode upon release
  buttonState = mockButton->getCurrentState();
  EXPECT_FALSE(buttonState.isPressed);
  EXPECT_FALSE(buttonState.isInAutoRepeatMode);
};

TEST_F(ButtonControllerTest, NotifyMediatorOnButtonPress) {
  // Simulate button press and process the state change
  mockButton->simulatePress(true);
  buttonController->processButtonStates();

  // Expect the mediator to be notified on button press
  EXPECT_CALL(*mockMediator,
              notify(buttonController.get(), EventType::BUTTON_PRESSED, _))
      .Times(1);
};

TEST_F(ButtonControllerTest, NotifyMediatorOnAutoRepeatEvents) {
  // Set the button to be continuously pressed for auto-repeat
  mockButton->simulatePress(true);
  buttonController->processButtonStates();
  
  // Simulate the initial delay for auto-repeat activation
  SimulateTimePassage(501);  // Wait for initial delay
  buttonController->processButtonStates();

  // Expect the mediator to be notified on auto-repeat events
  EXPECT_CALL(*mockMediator,
              notify(buttonController.get(), EventType::BUTTON_AUTO_REPEAT, _))
      .Times(AtLeast(1));

  // Simulate time passage for auto-repeat intervals
  SimulateTimePassage(200);  // Wait for two standard rate intervals
  buttonController->processButtonStates();
};

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
};

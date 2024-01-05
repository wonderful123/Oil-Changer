#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>

#include "ButtonBase.h"
#include "HardwarePinConfig.h"
#include "IButton.h"
#include "InteractionSettings.h"
#include "Mocks/MockButton.h"

using namespace std::chrono;

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

/**
 * Test Cases for ButtonBase and Derived Classes
 *
 * - ParsesValidButtonSettings: Checks if MockButton is correctly initialized
 * with HardwarePinConfig.
 * - PressedButtonChangesStateToPressed: Verifies that the button state changes
 * to 'pressed' upon a button press.
 * - ButtonReleaseChangesStateAfterPressed: Ensures that the button state
 * changes from 'pressed' to 'not pressed' upon release.
 * - PressedButtonInvokesCallback: Checks if pressing the button invokes the set
 * callback.
 * - ReleaseDetection: Verifies that the callback is not invoked upon button
 * release.
 * - AutoRepeatAfterInitialDelay: Tests if the button enters auto-repeat mode
 * after the initial delay.
 * - ExitAutoRepeatOnRelease: Checks if the button exits auto-repeat mode upon
 * release.
 *
 */

class ButtonTest : public ::testing::Test {
protected:
  HardwarePinConfig testConfig{1, "Button1", "Button"};
  std::shared_ptr<MockButton> mockButton;
  InteractionSettings::AutoRepeat autoRepeatSettings;

  void SimulateTimePassage(int milliseconds) {
    auto duration = std::chrono::milliseconds(milliseconds);
    // Set the mock button's internal state to simulate time passage
    mockButton->mockAdjustLastPressTime(duration);
  }

  void SetUp() {
    // Manually set up autoRepeat settings for the test
    autoRepeatSettings.initialDelayMs = 500;
    autoRepeatSettings.standardRateMs = 100;
    autoRepeatSettings.acceleration = {.startAfterMs = 2000,
                                       .rateDecreaseIntervalMs = 500,
                                       .minimumRateMs = 10};

    mockButton = std::make_shared<MockButton>(testConfig);

    mockButton->setHoldDurationThreshold(500);
    mockButton->setLongHoldDurationThreshold(1000);
  }
};

TEST_F(ButtonTest, ParsesValidButtonSettings) {
  // Arrange
  // (Setup is already done in the SetUp method)

  // Act
  // The button is initialized with testConfig in the SetUp method

  // Assert
  // Verify that the mockButton has the correct configuration
  // This assumes that ButtonBase or MockButton provides a way to check the
  // configuration
  EXPECT_EQ(mockButton->pinNumber(), testConfig.pinNumber);
  EXPECT_EQ(mockButton->id(), testConfig.id);
  EXPECT_EQ(mockButton->type(), testConfig.type);
};

TEST_F(ButtonTest, ButtonPressAndReleaseChangesState) {
  // Simulate button press
  mockButton->simulatePress(true);
  mockButton->updateButtonState();
  IButton::State state = mockButton->getState();
  EXPECT_EQ(IButton::State::Pressed, state);

  // Simulate button release
  mockButton->simulatePress(false);
  mockButton->updateButtonState();
  state = mockButton->getState();
  EXPECT_EQ(IButton::State::Released, state);
}

TEST_F(ButtonTest, HoldStateAfterDelay) {
  // Simulate button press
  mockButton->simulatePress(true);
  mockButton->updateButtonState();
  IButton::State state = mockButton->getState();
  EXPECT_EQ(IButton::State::Pressed, state);

  // Simulate passage of time to just beyond the initial delay
  SimulateTimePassage(501); // Assuming initial delay is 500ms
  mockButton->updateButtonState();

  // Check if the button is in held state after the initial delay
  state = mockButton->getState();
  EXPECT_EQ(IButton::State::Held, state);

  // Simulate a small amount of time less than long hold duration
  SimulateTimePassage(200);
  mockButton->updateButtonState(); // Process state change for auto-repeat

  // Confirm the button is still in held state
  state = mockButton->getState();
  EXPECT_EQ(IButton::State::Held, state);

  // Confirm button release
  mockButton->simulatePress(false);
  mockButton->updateButtonState();
  state = mockButton->getState();
  EXPECT_EQ(IButton::State::Released, state);
};

TEST_F(ButtonTest, LongHoldStateTest) {
  // Simulate button press
  mockButton->simulatePress(true);
  mockButton->updateButtonState();
  IButton::State state = mockButton->getState();
  EXPECT_EQ(IButton::State::Pressed, state);

  // Simulate passage of time to just beyond the initial delay
  SimulateTimePassage(501); // Assuming initial delay is 500ms
  mockButton->updateButtonState();

  // Check if the button is in held state after the initial delay
  state = mockButton->getState();
  EXPECT_EQ(IButton::State::Held, state);

  // Simulateamount of time more than long hold duration
  SimulateTimePassage(500);
  mockButton->updateButtonState(); // Process state change for auto-repeat

  // Confirm the button is still in held state
  state = mockButton->getState();
  EXPECT_EQ(IButton::State::HeldLong, state);

  // Confirm button release
  mockButton->simulatePress(false);
  mockButton->updateButtonState();
  state = mockButton->getState();
  EXPECT_EQ(IButton::State::Released, state);
};
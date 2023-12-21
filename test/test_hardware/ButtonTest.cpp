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

using ButtonPressCallback = std::function<void(const std::string &)>;

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
    autoRepeatSettings.rapidPressDebounceMs = 150;

    mockButton = std::make_shared<MockButton>(testConfig);

    mockButton->setAutoRepeatSettings(autoRepeatSettings);
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

TEST_F(ButtonTest, PressedButtonChangesStateToPressed) {
  // Simulate button press
  mockButton->simulatePress(true);

  // Call update to process the press
  mockButton->update();

  // Assert: Verify that the button state is 'pressed'
  EXPECT_TRUE(mockButton->isPressed());
}

TEST_F(ButtonTest, ButtonReleaseChangesStateAfterPressed) {
  // Simulate button press
  mockButton->simulatePress(true);
  mockButton->update();
  EXPECT_TRUE(mockButton->isPressed());

  // Simulate button release
  mockButton->simulatePress(false);
  mockButton->update();
  EXPECT_FALSE(mockButton->isPressed());
}

// Press invokes callback test
class MockCallback {
 public:
  MOCK_METHOD(void, Call, (const std::string &), ());
};

TEST_F(ButtonTest, PressedButtonInvokesCallback) {
  MockCallback mockCallback;

  // Set expectation: The callback should be called once with any string
  // argument
  EXPECT_CALL(mockCallback, Call(::testing::_)).Times(1);

  // Set the callback on the button
  mockButton->setOnPressCallback(
      [&](const std::string &id) { mockCallback.Call(id); });

  // Simulate button press
  mockButton->simulatePress(true);

  mockButton->update();
}

// ReleaseDetection Test
TEST_F(ButtonTest, ReleaseDetection) {
  // Setup: Set the callback and then simulate a button press
  bool callbackInvoked = false;
  mockButton->setOnPressCallback(
      [&callbackInvoked](const std::string &) { callbackInvoked = true; });

  mockButton->simulatePress(true);  // Simulate the button being pressed
  mockButton->update();             // Update to process the press

  // Assert: Verify that callback is invoked on press
  EXPECT_TRUE(callbackInvoked);

  // Reset the callbackInvoked flag
  callbackInvoked = false;

  // Simulate button release and update
  mockButton->simulatePress(false);  // Simulate the button being released
  mockButton->update();              // Update to process the release

  // Assert: Verify that no callback is invoked on release
  EXPECT_FALSE(callbackInvoked);
};

// Test auto-repeat functionality
TEST_F(ButtonTest, AutoRepeatAfterInitialDelay) {
  mockButton->simulatePress(true);
  mockButton->update();  // Process state change

  // Simulate passage of time to just beyond the initial delay
  SimulateTimePassage(501);  // Assuming initial delay is 500ms
  mockButton->update();

  // Check if the button is in auto-repeat mode after the initial delay
  EXPECT_TRUE(mockButton->getCurrentState().isInAutoRepeatMode);

  // Simulate time passage for the standard repeat rate
  SimulateTimePassage(201);  // Assuming standard rate is 200ms
  mockButton->update();      // Process state change for auto-repeat

  // Confirm the button is still in auto-repeat mode
  EXPECT_TRUE(mockButton->getCurrentState().isInAutoRepeatMode);
};

// Test to check button exits auto-repeat mode upon release
TEST_F(ButtonTest, ExitAutoRepeatOnRelease) {
  mockButton->simulatePress(true);
  mockButton->update();  // Process initial press

  // Simulate time passage just beyond the initial delay to enter auto-repeat
  // mode
  SimulateTimePassage(501);
  mockButton->update();

  // Confirm the button is in auto-repeat mode
  EXPECT_TRUE(mockButton->getCurrentState().isInAutoRepeatMode);

  // Simulate button release
  mockButton->simulatePress(false);
  mockButton->update();  // Process button release

  // Check if the button exits auto-repeat mode upon release
  EXPECT_FALSE(mockButton->getCurrentState().isInAutoRepeatMode);
};

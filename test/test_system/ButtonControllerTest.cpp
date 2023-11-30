/**
 * @file ButtonControllerTests.cpp
 * @brief Tests for the ButtonController class
 *
 * @section Test Cases
 *
 * The following test cases cover various aspects of the ButtonController class:
 *
 * 1. **ObserverRegistration:** Verifies that observers can be added to the
 * ButtonController.
 * 2. **DuplicateObserverRegistration:** Checks the behavior when the same
 * observer is added multiple times.
 * 3. **ButtonRegistration:** Ensures that buttons can be registered with their
 * respective IDs.
 * 4. **DuplicateButtonRegistration:** Verifies the behavior when the same
 * button is registered multiple times with different IDs.
 * 5. **NotifyObservers:** Confirms that all registered observers are notified
 * when a button is pressed.
 * 6. **NotifyWithNoObservers:** Checks the behavior when notifying without any
 * registered observers.
 * 7. **ButtonPressSimulation:** Simulates a button press and ensures that the
 * correct observer method is called with the correct ID.
 */

#include "ButtonController.h"
#include "IButtonControllerObserver.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "MockButtonController.h"
#include "Mocks/MockButton.h"
#include "HardwarePinConfig.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;

// Test Fixture for ButtonController
class ButtonControllerTest : public ::testing::Test {
protected:
  HardwarePinConfig testConfig{1, "Button1", "Button"};
  std::shared_ptr<MockButton> mockButton;
  ButtonController buttonController;
  std::shared_ptr<MockButtonControllerObserver> mockObserver;
  std::string testButtonId = "testButton";

  void SetUp() override {
    mockObserver = std::make_shared<MockButtonControllerObserver>();
    mockButton = std::make_shared<MockButton>(testConfig);
    buttonController.addObserver(mockObserver);
    buttonController.registerButton(testButtonId, mockButton);
  }
};

// Test Case: Observer can be added to ButtonController
TEST_F(ButtonControllerTest, ObserverRegistration) {
  EXPECT_CALL(*mockObserver, onButtonPress(_)).Times(1);
  buttonController.notifyObservers(testButtonId);
}

// Test Case: Check behavior when the same observer is added multiple times
TEST_F(ButtonControllerTest, DuplicateObserverRegistration) {
  buttonController.addObserver(mockObserver); // Duplicate registration
  EXPECT_CALL(*mockObserver, onButtonPress(_)).Times(1);
  buttonController.notifyObservers(testButtonId);
}

// Test Case: Buttons can be registered with their respective IDs
TEST_F(ButtonControllerTest, ButtonRegistration) {
  std::shared_ptr<MockButton> anotherButton =
      std::make_shared<MockButton>(testConfig);
  std::string anotherButtonId = "anotherButton";

  // Register another button
  buttonController.registerButton(anotherButtonId, anotherButton);

  // Expect the observer to be notified for the newly registered button
  EXPECT_CALL(*mockObserver, onButtonPress(anotherButtonId)).Times(1);

  // Trigger the button press event for the newly registered button
  buttonController.notifyObservers(anotherButtonId);
}

// Test Case: Verify that all registered observers are notified when a button is
// pressed
TEST_F(ButtonControllerTest, NotifyObservers) {
  // Expect the observer to be notified for the test button press
  EXPECT_CALL(*mockObserver, onButtonPress(testButtonId)).Times(1);

  // Trigger the button press event
  buttonController.notifyObservers(testButtonId);
}

// Test Case: Check the behavior when notifying without any registered observers
TEST_F(ButtonControllerTest, NotifyWithNoObservers) {
  ButtonController freshButtonController;
  freshButtonController.registerButton(testButtonId, mockButton);
  freshButtonController.notifyObservers(
      testButtonId); // No observers, expecting no action
}

TEST_F(ButtonControllerTest, DuplicateButtonRegistration) {
  std::string anotherButtonId = "anotherButton";

  // Register the same button with a different ID
  buttonController.registerButton(anotherButtonId, mockButton);

  // Expect the observer to be notified for both button IDs
  EXPECT_CALL(*mockObserver, onButtonPress(testButtonId)).Times(1);
  EXPECT_CALL(*mockObserver, onButtonPress(anotherButtonId)).Times(1);

  // Trigger the button press event for both button IDs
  buttonController.notifyObservers(testButtonId);
  buttonController.notifyObservers(anotherButtonId);
}

TEST_F(ButtonControllerTest, ButtonPressHandling) {
  // Expect the observer to be notified with the correct button ID
  EXPECT_CALL(*mockObserver, onButtonPress(testButtonId)).Times(1);

  // Trigger the button press event
  buttonController.notifyObservers(testButtonId);
}

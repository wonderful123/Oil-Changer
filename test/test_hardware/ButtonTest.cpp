#include "ButtonBase.h"
#include "HardwarePinConfig.h"
#include "IButton.h"
#include "Mocks/MockButton.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class ButtonTest : public ::testing::Test {
protected:
  HardwarePinConfig testConfig{1, "Button1", "Button"};
  std::shared_ptr<MockButton> mockButton;

  void SetUp() override {
    mockButton = std::make_shared<MockButton>(testConfig);
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
}

TEST_F(ButtonTest, PressDetection) {
  // Setup: Simulate a button press by setting an expectation
  EXPECT_CALL(*mockButton, isPressed()).WillOnce(Return(true));

  // Act: Call the method that checks if the button is pressed
  bool pressed = mockButton->isPressed();

  // Assert: Verify that isPressed() returns true
  EXPECT_TRUE(pressed);
}

TEST_F(ButtonTest, ReleaseDetection) {
  // Setup: Simulate a button release by setting an expectation
  EXPECT_CALL(*mockButton, isPressed()).WillOnce(Return(false));

  // Act: Call the method that checks if the button is pressed
  bool pressed = mockButton->isPressed();

  // Assert: Verify that isPressed() returns false
  EXPECT_FALSE(pressed);
}

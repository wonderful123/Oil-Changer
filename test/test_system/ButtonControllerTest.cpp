/**
 * @file ButtonControllerTests.cpp
 * @brief Tests for the ButtonController class
 *
 * @section Test Cases
 *
 * The following test cases cover various aspects of the ButtonController class:
 *
 **/

#include "ButtonController.h"
#include "HardwarePinConfig.h"
#include "Mediator/ConcreteMediator.h"
#include "MockButtonController.h"
#include "Mocks/MockButton.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

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
  std::shared_ptr<ButtonController> buttonController;
  std::string testButtonId = "testButton";

  void SetUp() override {
    auto mediator = std::make_shared<ConcreteMediator>();
    buttonController = std::make_shared<ButtonController>(mediator);
    mockButton = std::make_shared<MockButton>(testConfig);
  }
};

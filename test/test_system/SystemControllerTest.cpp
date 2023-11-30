#include "SystemController.h"
#include "ButtonController.h"
#include "HardwareManager.h"
#include "MockButtonController.h"
#include "MockConfigManager.h"
#include "MockHardwareFactory.h"
#include "MockHardwareManager.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class SystemControllerTest : public ::testing::Test {
protected:
  std::shared_ptr<MockFileHandler> mockFileHandler;
  std::shared_ptr<MockButtonController> mockButtonController;
  std::shared_ptr<MockConfigManager> mockConfigManager;
  std::shared_ptr<MockHardwareFactory> mockHardwareFactory;
  std::shared_ptr<MockHardwareManager> mockHardwareManager;
  std::unique_ptr<SystemController> systemController;

  void SetUp() override {
    mockFileHandler = std::make_shared<MockFileHandler>();
    mockButtonController = std::make_shared<MockButtonController>();

    // Correct the instantiation of MockConfigManager
    mockConfigManager =
        std::make_shared<MockConfigManager>(mockFileHandler.get());

    // mockHardwareFactory = std::make_shared<MockHardwareFactory>();
    auto &hardwareFactory = HardwareFactory::getHardwareFactory();

    mockHardwareManager.reset(new MockHardwareManager(
        mockConfigManager, mockHardwareFactory, mockButtonController));

    systemController.reset(
        new SystemController(mockHardwareManager, mockButtonController));
  }
};

TEST_F(SystemControllerTest, ButtonPressTriggersBuzzer) {
  // Arrange
  // Set up expectations
  // Expect the buzzer to be triggered in the HardwareManager
  EXPECT_CALL(*mockHardwareManager, triggerBuzzer()).Times(1);

  // Act
  // Simulate a button press
  systemController->onButtonPress("testButtonId");

  // Assertions are handled by the EXPECT_CALL statements
}
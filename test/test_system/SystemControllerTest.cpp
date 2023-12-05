#include "SystemController.h"
#include "ButtonController.h"
#include "DIContainer.h"
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
    mockConfigManager = std::make_shared<MockConfigManager>();
    mockHardwareFactory = std::make_shared<MockHardwareFactory>();
    mockHardwareManager = std::make_shared<MockHardwareManager>();

    // Register mock dependencies in the DI Container
    DIContainer::clear(); // Clear existing instances in DIContainer
    DIContainer::registerInstance(mockFileHandler);
    DIContainer::registerInstance(mockConfigManager);
    DIContainer::registerInstance(mockHardwareFactory);
    DIContainer::registerInstance(mockButtonController);
    DIContainer::registerInstance(mockHardwareManager);

    // Resolve SystemController using DIContainer
    systemController = DIContainer::resolve<SystemController>();
  }

  void TearDown() override {
    DIContainer::clear(); // Clear DIContainer after each test
  }
};

// TEST_F(SystemControllerTest, ButtonPressTriggersBuzzer) {
//   // Arrange
//   // Set up expectations
//   // Expect the buzzer to be triggered in the HardwareManager
//   EXPECT_CALL(*mockHardwareManager, triggerBuzzer()).Times(1);

//   // Act
//   // Simulate a button press
//   systemController->onButtonPress("testButtonId");

//   // Assertions are handled by the EXPECT_CALL statements
// }
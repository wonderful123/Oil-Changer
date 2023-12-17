#include <gtest/gtest.h>

#include <memory>

#include "MockButtonController.h"
#include "MockConfigManager.h"
#include "MockHardwareFactory.h"
#include "MockHardwareManager.h"
#include "MockMediator.h"
#include "Mocks/MockFileHandler.h"
#include "SystemController.h"

using ::testing::_;
using ::testing::Invoke;

#include "FSM/StateMachine.h"
#include "FSM/States.h"

class SystemControllerTest : public ::testing::Test {
 protected:
  std::shared_ptr<MockMediator> mockMediator;
  std::shared_ptr<MockFileHandler> mockFileHandler;
  std::shared_ptr<MockConfigManager> mockConfigManager;
  std::shared_ptr<MockHardwareFactory> mockHardwareFactory;
  std::shared_ptr<MockButtonController> mockButtonController;
  std::shared_ptr<MockHardwareManager> mockHardwareManager;
  std::unique_ptr<SystemController> systemController;

  void SetUp() {
    mockMediator = std::make_shared<MockMediator>();
    mockFileHandler = std::make_shared<MockFileHandler>();
    mockConfigManager =
        std::make_shared<MockConfigManager>(mockMediator, mockFileHandler);
    mockHardwareFactory = std::make_shared<MockHardwareFactory>();
    mockButtonController = std::make_shared<MockButtonController>(mockMediator);

    // Use std::static_pointer_cast to cast mocks to their base class types
    mockHardwareManager = std::make_shared<MockHardwareManager>(
        mockMediator, mockConfigManager, mockHardwareFactory,
        mockButtonController);

    systemController = std::unique_ptr<SystemController>(new SystemController(
        mockMediator, mockButtonController, mockHardwareManager));

    ASSERT_TRUE(systemController != nullptr)
        << "SystemController failed to initialize";
  }
};

TEST_F(SystemControllerTest, ButtonPressTriggersBuzzer) {
  EventType eventType = EventType::BUTTON_PRESSED;
  EventData eventData;
  eventData.id = "testButton";
  EXPECT_CALL(*mockHardwareManager, triggerBuzzer()).Times(1);
  systemController->receiveEvent(eventType, &eventData);
}
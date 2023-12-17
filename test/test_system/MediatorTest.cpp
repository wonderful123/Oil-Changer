#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

#include "ConcreteMediator.h"
#include "EventData.h"
#include "IColleague.h"
#include "MockMediator.h"

// Mock class for IColleague
class MockColleague : public IColleague {
 public:
  explicit MockColleague(std::shared_ptr<IMediator> mediator)
      : IColleague(mediator) {}

  MOCK_METHOD(void, receiveEvent,
              (EventType eventType, const EventData *eventData), (override));
};

// Test Fixture
class MediatorTest : public ::testing::Test {
 protected:
  std::shared_ptr<ConcreteMediator> mediator;
  std::shared_ptr<MockColleague> colleague1;
  std::shared_ptr<MockColleague> colleague2;

  void SetUp() override {
    mediator = std::make_shared<ConcreteMediator>();
    colleague1 = std::make_shared<MockColleague>(mediator);
    colleague2 = std::make_shared<MockColleague>(mediator);

    mediator->registerColleague(colleague1.get());
    mediator->registerColleague(colleague2.get());
  }

  void TearDown() override {
    mediator->deregisterColleague(colleague1.get());
    mediator->deregisterColleague(colleague2.get());
  }
};

// Test that colleagues are correctly registered in the mediator
TEST_F(MediatorTest, ColleaguesAreRegistered) {
  // Assuming ConcreteMediator has a method to check if a colleague is
  // registered
  EXPECT_TRUE(mediator->isColleagueRegistered(colleague1.get()));
  EXPECT_TRUE(mediator->isColleagueRegistered(colleague2.get()));
}

// Test the notify functionality of the mediator
TEST_F(MediatorTest, NotifyFunctionality) {
  EventType testEventType = EventType::BUTTON_PRESSED;
  EventData testData;  // Replace with actual event data if needed

  // Set expectation that when the mediator notifies, only colleague2 receives
  // the event
  EXPECT_CALL(*colleague2, receiveEvent(testEventType, &testData)).Times(1);

  // Trigger the event from colleague1
  mediator->notify(colleague1.get(), testEventType, &testData);
}

// Test the deregister functionality of the mediator
TEST_F(MediatorTest, DeregisterColleagues) {
  EventType testEventType = EventType::BUTTON_PRESSED;
  EventData testData;

  // Deregister colleague1
  mediator->deregisterColleague(colleague1.get());

  // Set expectations
  EXPECT_CALL(*colleague2, receiveEvent(testEventType, &testData))
      .Times(1)
      .WillOnce(
          testing::Invoke([](EventType eventType, const EventData *eventData) {
            // Optional: Add diagnostic output or breakpoint here
            std::cout << "Colleague2 received event\n";
          }));

  // Trigger the event from colleague1 (which is deregistered)
  mediator->notify(colleague1.get(), testEventType, &testData);
}

// Test the mediator's behavior when a colleague sends an event
TEST_F(MediatorTest, ColleagueSendsEvent) {
  EventType testEventType = EventType::BUTTON_PRESSED;
  EventData testData;

  // Set expectation that colleague1 should receive the event since colleague2
  // is the sender
  EXPECT_CALL(*colleague1, receiveEvent(testEventType, &testData)).Times(1);

  // Set expectation that colleague2 should not receive its own event
  EXPECT_CALL(*colleague2, receiveEvent(testEventType, &testData)).Times(0);

  // Trigger the event from colleague2
  mediator->notify(colleague2.get(), testEventType, &testData);
}

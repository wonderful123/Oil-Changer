#pragma once

#include <gmock/gmock.h>

#include "Mediator/Event.h"
#include "Mediator/IMediator.h"

class MockMediator : public IMediator {
 public:
  MOCK_METHOD(void, registerColleague, (IColleague *), (override));
  MOCK_METHOD(void, notify, (const IColleague *, EventType, const EventData *),
              (override));
};

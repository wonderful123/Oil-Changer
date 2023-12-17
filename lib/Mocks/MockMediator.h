#include <gmock/gmock.h>

#include "EventType.h"
#include "IMediator.h"

class MockMediator : public IMediator {
 public:
  MOCK_METHOD(void, registerColleague, (IColleague *), (override));
  MOCK_METHOD(void, notify, (const IColleague *, EventType, const EventData *),
              (override));
};

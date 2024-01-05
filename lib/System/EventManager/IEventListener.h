#pragma once

#include "Event.h"
#include <string>

class IEventListener {
public:
  virtual ~IEventListener() = default;
  virtual void onNotify(EventType eventType, const EventData &eventData) = 0;
};

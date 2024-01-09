// IEventListener.h
#pragma once

#include "Event.h"
#include <string>

class IEventListener {
public:
  virtual ~IEventListener() = default;
  virtual void onNotify(Event type, Parameter parameter){};
  virtual void onNotify(Event type, Parameter parameter, float value){};
  virtual void onNotify(Event type, Parameter parameter,
                        const std::string &message){};
};

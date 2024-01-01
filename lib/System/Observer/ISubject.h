#pragma once

#include "IObserver.h"
#include <vector>

class ISubject {
public:
  virtual ~ISubject() = default;
  virtual void attach(IObserver *observer) = 0;
  virtual void detach(IObserver *observer) = 0;
  virtual void notify(const std::string &event, const std::string &id) = 0;
};

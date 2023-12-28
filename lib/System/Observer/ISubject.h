#pragma once

#include "IObserver.h"
#include <memory>
#include <vector>

class ISubject {
public:
  virtual ~ISubject() = default;
  virtual void attach(std::shared_ptr<IObserver> observer) = 0;
  virtual void detach(std::shared_ptr<IObserver> observer) = 0;
  virtual void notify(const std::string &event, const std::string &id) = 0;
};

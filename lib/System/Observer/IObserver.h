#pragma once

#include <string>

class IObserver {
public:
  virtual ~IObserver() = default;
  virtual void onNotify(const std::string &event, const std::string &id) = 0;
};

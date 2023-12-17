#pragma once

#include "IColleague.h"
#include <algorithm>
#include <mutex>
#include <vector>

class ConcreteMediator : public IMediator {
  std::vector<IColleague *> colleagues;
  std::mutex mutex;

public:
  void registerColleague(IColleague *colleague) {
    std::lock_guard<std::mutex> lock(
        mutex); // Use lock if thread safety is needed.
    if (std::find(colleagues.begin(), colleagues.end(), colleague) ==
        colleagues.end()) {
      colleagues.push_back(colleague);
    }
  }

  void deregisterColleague(IColleague *colleague) {
    std::lock_guard<std::mutex> lock(
        mutex); // Use lock if thread safety is needed.
    colleagues.erase(
        std::remove(colleagues.begin(), colleagues.end(), colleague),
        colleagues.end());
  }

  void notify(const IColleague *sender, EventType eventType,
              const EventData *data = nullptr) override {
    for (auto &colleague : colleagues) {
      if (colleague != sender) {
        colleague->receiveEvent(eventType, data);
      }
    }
  }

  bool isColleagueRegistered(IColleague *colleague) {
    std::lock_guard<std::mutex> lock(mutex); // Use lock for thread safety
    return std::find(colleagues.begin(), colleagues.end(), colleague) !=
           colleagues.end();
  }
};
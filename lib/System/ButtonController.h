#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Observer/ISubject.h"

class IButton;

class ButtonController : public ISubject {
public:
  virtual void registerButton(const std::string &id,
                              std::shared_ptr<IButton> button);
  virtual void processButtonStates();
  virtual std::shared_ptr<IButton> getButtonById(const std::string &id) const;

  void attach(IObserver *observer) override;
  void detach(IObserver *observer) override;
  void notify(const std::string &event, const std::string &id) override;

private:
  std::unordered_map<std::string, std::shared_ptr<IButton>> _buttons;
  std::vector<IObserver *> _observers;
};

#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

#include "IButton.h"
#include "InteractionSettings.h"
#include "Observer/IObserver.h"
#include "Observer/ISubject.h"

class ButtonController : public ISubject {
public:
  virtual void registerButton(const std::string &id,
                              std::shared_ptr<IButton> button);
  virtual void processButtonStates();
  virtual std::shared_ptr<IButton> getButtonById(const std::string &id) const;

  void attach(std::shared_ptr<IObserver> observer) override;
  void detach(std::shared_ptr<IObserver> observer) override;
  void notify(const std::string &event, const std::string &id) override;

private:
  std::unordered_map<std::string, std::shared_ptr<IButton>> _buttons;
  std::vector<std::shared_ptr<IObserver>> _observers;
};

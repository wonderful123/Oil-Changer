#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

#include "InteractionSettings.h"
#include "Mediator/IColleague.h"

class IButton;

class ButtonController : public IColleague {
 public:
  struct ButtonState {
    std::shared_ptr<IButton> button;
    bool isPressed;
    std::chrono::steady_clock::time_point lastPressTime;
    std::chrono::steady_clock::time_point lastRepeatTime;
    bool isInAutoRepeatMode;
  };

  ButtonController(std::shared_ptr<IMediator> mediator);

  virtual void registerButton(const std::string &id,
                              std::shared_ptr<IButton> button);
  virtual void processButtonStates();
  virtual std::shared_ptr<IButton> getButtonById(const std::string &id) const;
  virtual void setInteractionSettings(const InteractionSettings &settings);

  virtual void receiveEvent(EventType eventType,
                            const EventData *eventData) override;

 private:
  std::unordered_map<std::string, std::shared_ptr<IButton>> _buttons;
  std::unordered_map<std::string, ButtonState> _buttonStates;
  std::shared_ptr<IMediator> _mediator;
  InteractionSettings _settings;

  void handleButtonPress(const std::string &id);
  void notifyMediator(const std::string &id);
  void handleAutoRepeat(const std::string &id, ButtonState &state);
};

#pragma once

#include "FSM/StateMachine.h"
#include "Mediator/IColleague.h"

class HardwareManager;
class ButtonController;

class SystemController : public IColleague {
 public:
  SystemController(std::shared_ptr<IMediator> mediator,
                   std::shared_ptr<ButtonController> buttonController,
                   std::shared_ptr<HardwareManager> hardwareManager);

  virtual void receiveEvent(EventType eventType,
                            const EventData *eventData) override;
  void handleEvent(const EventType &eventType);

  virtual void update(EventType eventType);

  void performPeriodicUpdate();

 private:
  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<ButtonController> _buttonController;
  StateMachine _stateMachine;
  std::shared_ptr<IMediator> _mediator;

  void onButtonPress(const std::string &id);
};

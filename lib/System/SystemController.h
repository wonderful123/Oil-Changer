#pragma once

#include "AutoRepeatHandler.h"
#include "ButtonController.h"
#include "BuzzerManager.h"
#include "ConfigManager.h"
#include "FSM/StateMachine.h"
#include "HardwareManager.h"
#include "Mediator/IColleague.h"

class SystemController : public IColleague {
public:
  SystemController(std::shared_ptr<IMediator> mediator,
                   std::shared_ptr<HardwareManager> hardwareManager);

  void initializeSystemComponents();

  virtual void receiveEvent(EventType eventType,
                            const EventData *eventData) override;
  void handleEvent(const EventType &eventType);

  virtual void update(EventType eventType);

  void performPeriodicUpdate();

private:
  StateMachine _stateMachine;
  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<BuzzerManager> _buzzerManager;
  std::shared_ptr<IMediator> _mediator;
  std::shared_ptr<AutoRepeatHandler> _autoRepeatHandler;

  Error initializeButtonController(InteractionSettings interactionSettings);
  Error initializeBuzzerManager(InteractionSettings interactionSettings);

  void onButtonPress(const std::string &id);
};

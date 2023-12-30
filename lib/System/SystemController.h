#pragma once

#include "Error.h"
#include "FSM/StateMachine.h"
#include "Mediator/IMediator.h"

class ButtonController;
class HardwareManager;
class AutoRepeatHandler;
class BuzzerManager;
class ConfigManager;
class InteractionSettings;

class SystemController : public IColleague {
public:
  SystemController(std::shared_ptr<IMediator> mediator,
                   std::shared_ptr<HardwareManager> hardwareManager);

  void initializeSystemComponents();
  bool loadInteractionSettings();

  virtual void receiveEvent(EventType eventType,
                            const EventData *eventData) override;
  virtual void update(EventType eventType);

  void performPeriodicUpdate();

  std::shared_ptr<ButtonController> getButtonController();
  std::shared_ptr<BuzzerManager> getBuzzerManager();

private:
  StateMachine _stateMachine;
  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<BuzzerManager> _buzzerManager;
  std::shared_ptr<IMediator> _mediator;
  std::shared_ptr<AutoRepeatHandler> _autoRepeatHandler;
  std::shared_ptr<InteractionSettings> _interactionSettings;

  void initializeAutoRepeatHandler(
      std::shared_ptr<InteractionSettings> &interactionSettings);
  Error initializeButtonController();
  Error initializeBuzzerManager(
      std::shared_ptr<InteractionSettings> &interactionSettings);

  void onButtonPress(const std::string &id);
};

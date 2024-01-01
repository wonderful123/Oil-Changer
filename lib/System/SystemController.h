#pragma once

#include "Error.h"
#include "Mediator/IMediator.h"

class StateMachine;
class ButtonController;
class HardwareManager;
class AutoRepeatHandler;
class BuzzerManager;
class ConfigManager;
class InteractionSettings;

class SystemController {
public:
  SystemController(std::shared_ptr<IMediator> mediator,
                   std::shared_ptr<HardwareManager> hardwareManager);

  void initializeSystemComponents();
  bool loadInteractionSettings();

  void performPeriodicUpdate();

  std::shared_ptr<ButtonController> getButtonController();
  std::shared_ptr<BuzzerManager> getBuzzerManager();

private:
  std::shared_ptr<StateMachine> _stateMachine;
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
};

#pragma once

#include "Error.h"
#include <memory>

class StateMachine;
class ButtonController;
class HardwareManager;
class BuzzerManager;
class ConfigManager;
class InteractionSettings;

class SystemController {
public:
  SystemController(std::shared_ptr<HardwareManager> hardwareManager);

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
  std::shared_ptr<InteractionSettings> _interactionSettings;

  Error initializeButtonController(
      std::shared_ptr<InteractionSettings> &interactionSettings);
  Error initializeBuzzerManager(
      std::shared_ptr<InteractionSettings> &interactionSettings);
};

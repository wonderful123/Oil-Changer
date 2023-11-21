#pragma once

#include "ButtonController.h"
#include "FSM/StateMachine.h"
#include "HardwareManager.h"
#include "IButtonControllerObserver.h"

class SystemController : public IButtonControllerObserver,
                         public std::enable_shared_from_this<SystemController> {
public:
  SystemController(std::shared_ptr<HardwareManager> hardwareManager,
                   std::shared_ptr<ButtonController> buttonController);

  void initialize();
  void onButtonPress(int buttonId) override;

  void startSystem();
  void stopSystem();
  void handleEvent(const Event &event);

private:
  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<ButtonController> _buttonController;
  StateMachine _stateMachine;

  void changeSystemState();
  void registerAsButtonObserver();
};

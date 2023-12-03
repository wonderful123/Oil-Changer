#pragma once

#include "ButtonController.h"
#include "FSM/Events.h"
#include "FSM/StateMachine.h"
#include "HardwareManager.h"
#include "IButtonControllerObserver.h"

class SystemController : public IButtonControllerObserver,
                         public std::enable_shared_from_this<SystemController> {
public:
  SystemController(std::shared_ptr<HardwareManager> hardwareManager,
                   std::shared_ptr<ButtonController> buttonController);

  virtual void initialize();
  virtual void onButtonPress(const std::string &id) override;

  virtual void update(EventType eventType);

private:
  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<ButtonController> _buttonController;
  StateMachine _stateMachine;

  void registerAsButtonObserver();
};

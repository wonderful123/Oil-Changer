#pragma once

#include <tinyfsm.hpp>

#include "Mediator/IMediator.h"
#include "Observer/IObserver.h"
#include "StateMachine.h"
#include <memory>

class BuzzerManager;
class SystemFactory;
class ButtonController;
class Extracting;

class Ready : public StateMachine,
              public IObserver,
              public IColleague {
public:
  Ready();
  void entry() override;
  void exit() override;

private:
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<BuzzerManager> _buzzerManager;
  std::shared_ptr<IMediator> _mediator;

  void handleButtonPress(const std::string &buttonId);
  void onNotify(const std::string &event, const std::string &buttonId);
  void setupBuzzerRapidBeepCallback();
};
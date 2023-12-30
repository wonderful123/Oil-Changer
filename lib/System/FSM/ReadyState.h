#include <tinyfsm.hpp>

#include "Observer/IObserver.h"
#include "Mediator/IMediator.h"
#include "ButtonController.h"
#include "BuzzerManager.h"
#include "SystemFactory.h"
#include <memory>

class StateMachine;
class Extracting;

class Ready : public StateMachine,
              public IObserver,
              public IColleague,
              public std::enable_shared_from_this<Ready> {
public:
  void entry() override;
  void exit() override;

private:
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<BuzzerManager> _buzzerManager;
  std::shared_ptr<IMediator> _mediator;

  void handleButtonPress(const std::string &id);
  void setupBuzzerRapidBeepCallback();
};
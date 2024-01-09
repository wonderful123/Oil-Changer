// StateMachine.h
#pragma once

#include "BuzzerManager.h"
#include "EventManager/EventManager.h"
#include "Events.h"
#include <tinyfsm.hpp>

class StateMachine : public tinyfsm::Fsm<StateMachine> {
protected:
  static std::shared_ptr<EventManager> _eventManager;
  static std::shared_ptr<BuzzerManager> _buzzerManager;

public:
  static void setSharedResources(std::shared_ptr<EventManager> em,
                                 std::shared_ptr<BuzzerManager> bm) {
    _eventManager = em;
    _buzzerManager = bm;
  }

  // Default reaction for unhandled events
  void react(tinyfsm::Event const &) {}

  virtual void react(ButtonPressEvent const &) {}
  virtual void react(ButtonReleaseEvent const &) {}
  virtual void react(ButtonHoldEvent const &) {}
  virtual void react(ButtonLongHoldEvent const &) {}

  virtual void react(InitializationEvent const &) {}
  virtual void react(InitializationCompleteEvent const &) {}

  virtual void react(StartOilChangeEvent const &) {}

  virtual void react(OilExtractionEvent const &) {}
  virtual void react(OilFillingEvent const &) {}
  virtual void react(ConfigurationModeEvent const &) {}
  virtual void react(ErrorDetectedEvent const &) {}
  virtual void react(UserCancelledEvent const &) {}
  virtual void react(TimeoutEvent const &) {}
  virtual void react(MaintenanceNeededEvent const &) {}

  // Entry and exit functions for states (optional)
  virtual void entry(void) {}
  virtual void exit(void) {}
};

using stateMachine = tinyfsm::FsmList<StateMachine>;

template <typename E> void sendFSMEvent(E const &event) {
  stateMachine::template dispatch<E>(event);
}
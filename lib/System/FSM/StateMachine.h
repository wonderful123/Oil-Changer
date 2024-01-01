#pragma once

#include <tinyfsm.hpp>

#include "Events.h"

class StateMachine : public tinyfsm::Fsm<StateMachine> {
public:
  // Default reaction for unhandled events
  void react(tinyfsm::Event const &) {}

  // Reaction functions for base event types
  virtual void react(ButtonPressEvent const &) {}
  virtual void react(InitializationEvent const &) {}
  virtual void react(InitializationCompleteEvent const &) {}
  virtual void react(OilCapacityEvent const &) {}
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
#include "SystemController.h"

#include "ButtonController.h"
#include "FSM/StateMachine.h"
#include "FSM/States.h"
#include "HardwareFactory.h"
#include "HardwareManager.h"
#include "Logger.h"

FSM_INITIAL_STATE(StateMachine, Ready);

SystemController::SystemController(
    std::shared_ptr<IMediator> mediator,
    std::shared_ptr<ButtonController> buttonController,
    std::shared_ptr<HardwareManager> hardwareManager)
    : IColleague(mediator),
      _mediator(mediator),
      _buttonController(buttonController),
      _hardwareManager(hardwareManager) {
  _mediator->registerColleague(this);
  _stateMachine.start();  // Initialize the state machine
}

void SystemController::handleEvent(const EventType &eventType) {
  // Handle events communicated by the mediator
  // Process the event as needed
}

void SystemController::receiveEvent(EventType eventType,
                                    const EventData *eventData) {
  // Handle events communicated by the mediator
  if (eventType == EventType::BUTTON_PRESSED) {
    onButtonPress(eventData->id);
  }
  // Other event types can be handled here as needed
}

void SystemController::onButtonPress(const std::string &id) {
  // Handle button press event
  _hardwareManager->triggerBuzzer();
  ButtonPressEvent event(id);
  _stateMachine.dispatch(event);
}

void SystemController::update(EventType eventType) {
  // Notify the mediator about the event
  _mediator->notify(this, eventType);
}

void SystemController::performPeriodicUpdate() {
  _buttonController->processButtonStates();
}
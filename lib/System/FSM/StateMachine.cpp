// StateMachine.cpp
#include "StateMachine.h"

// Define the static members
std::shared_ptr<EventManager> StateMachine::_eventManager;
std::shared_ptr<BuzzerManager> StateMachine::_buzzerManager;
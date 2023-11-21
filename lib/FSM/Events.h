#pragma once
#include <tinyfsm.hpp>

// Base event class
struct Event : tinyfsm::Event {};

// Define specific events
struct ButtonPressEvent : Event {
  int buttonId;
  ButtonPressEvent(int id) : buttonId(id) {}
};
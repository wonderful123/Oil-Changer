#pragma once

#include <string>
#include <tinyfsm.hpp>

// Event representing a button press
struct ButtonPressEvent : tinyfsm::Event {
  std::string buttonId;
  explicit ButtonPressEvent(const std::string &id) : buttonId(id) {}
};
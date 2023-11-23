#pragma once
#include <string>
#include <tinyfsm.hpp>

struct ButtonPressEvent : tinyfsm::Event {
  std::string buttonId;
  explicit ButtonPressEvent(const std::string &id) : buttonId(id) {}
};
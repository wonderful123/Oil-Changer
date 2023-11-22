#pragma once
#include <tinyfsm.hpp>

struct Event : tinyfsm::Event {
  virtual ~Event() = default;
};

struct ButtonPressEvent : Event {
  const std::string &buttonId;
  ButtonPressEvent(const std::string &id) : buttonId(id) {}
};

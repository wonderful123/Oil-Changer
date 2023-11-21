#pragma once
#include <tinyfsm.hpp>

struct Event : tinyfsm::Event {
  virtual ~Event() = default;
};

struct ButtonPressEvent : Event {
  int buttonId;
  ButtonPressEvent(int id) : buttonId(id) {}
};

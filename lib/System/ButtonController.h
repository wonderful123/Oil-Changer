#pragma once

#include "IButton.h"
#include <memory>
#include <unordered_map>
#include <vector>

class InteractionSettings;

enum class ButtonId { Plus, Minus, Start, Stop, ManualExtract, ManualFill };

struct ButtonData {
  std::shared_ptr<IButton> button;
  std::string idString;
};

extern std::unordered_map<std::string, ButtonId> buttonIdMap;

class ButtonController {
public:
  ButtonController(std::shared_ptr<InteractionSettings> &settings);
  virtual void registerButton(std::shared_ptr<IButton> button);
  virtual void processButtonStates();
  virtual std::shared_ptr<IButton> getButtonById(const std::string &id) const;

private:
  std::shared_ptr<InteractionSettings> _interactionSettings;
  std::unordered_map<ButtonId, std::shared_ptr<IButton>> _buttons;
  std::unordered_map<ButtonId, IButton::State> _previousButtonStates;
};

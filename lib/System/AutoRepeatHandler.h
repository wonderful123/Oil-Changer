#pragma once

#include "ButtonController.h"
#include "InteractionSettings.h"
#include "Observer/IObserver.h"
#include <chrono>
#include <unordered_map>

class AutoRepeatHandler : public IObserver {
public:
  explicit AutoRepeatHandler(std::shared_ptr<ButtonController> controller,
                             const InteractionSettings &settings);
  virtual void onNotify(const std::string &event,
                        const std::string &id) override;

private:
  std::shared_ptr<ButtonController> _controller;
  std::unordered_map<std::string, std::chrono::steady_clock::time_point>
      _lastPressTime;
  std::unordered_map<std::string, bool>
      _isInAutoRepeatMode; // Track auto-repeat mode for each button
  InteractionSettings _settings;

  void handleButtonPressed(const std::string &id);
  void handleButtonReleased(const std::string &id);
  void checkAutoRepeat(const std::string &id);
};

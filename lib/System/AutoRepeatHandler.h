#pragma once

#include "ButtonController.h"
#include "InteractionSettings.h"
#include "Observer/IObserver.h"

#include <chrono>
#include <unordered_map>
#include <unordered_set>

class AutoRepeatHandler : public IObserver {
public:
  explicit AutoRepeatHandler(std::shared_ptr<ButtonController> controller,
                             std::shared_ptr<InteractionSettings> &settings);
  virtual void onNotify(const std::string &event,
                        const std::string &buttonId) override;
  void checkAutoRepeat(); // Call this periodically

private:
  std::shared_ptr<ButtonController> _controller;
  std::shared_ptr<InteractionSettings> _settings;
  std::unordered_map<std::string, std::chrono::steady_clock::time_point>
      _lastPressTime;
  std::unordered_map<std::string, InteractionSettings::AutoRepeat>
      _cachedAutoRepeatSettings;
  std::unordered_set<std::string> _buttonsInAutoRepeatMode;
  std::unordered_map<std::string, bool>
      _isInAutoRepeatMode; // Track auto-repeat mode for each button
  std::unordered_map<std::string, bool>
      _autoRepeatNotified; // Track if already notified

  void handleButtonPressed(const std::string &buttonId);
  void handleButtonReleased(const std::string &buttonId);
};

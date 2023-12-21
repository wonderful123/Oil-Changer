#pragma once

#ifdef PLATFORM_NATIVE

#include <gmock/gmock.h>

#include "ButtonBase.h"

class MockButton : public ButtonBase {
 public:
  explicit MockButton(const HardwarePinConfig& config) : ButtonBase(config) {
    setInitialized(true);  // Assuming ButtonBase has a setInitialized method
  }

  // Method to simulate the button being pressed or not
  void simulatePress(bool pressed) { _isPressed = pressed; }

  MOCK_METHOD(void, updateButtonState, (), (override));

  // This override allows the mock to capture settings applied to it
  void setAutoRepeatSettings(
      const InteractionSettings::AutoRepeat& settings) override {
    currentSettings = settings;
    ButtonBase::setAutoRepeatSettings(
        settings);  // Call base class implementation
  }

  // For testing: Get the applied auto-repeat settings
  const InteractionSettings::AutoRepeat& getAppliedSettings() const {
    return currentSettings;
  }

  void mockAdjustLastPressTime(std::chrono::milliseconds duration) {
    adjustLastPressTimeForTesting(duration);
  }

 private:
  InteractionSettings::AutoRepeat currentSettings;
};

#endif  // PLATFORM_NATIVE

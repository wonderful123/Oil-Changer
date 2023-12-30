#pragma once

#include "HardwareComponent.h"
#include <functional>
#include <memory>

class InteractionSettings;

class IBuzzer : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent;

  virtual ~IBuzzer() = default;

  virtual void
  updateSettings(std::shared_ptr<InteractionSettings> &settings) = 0;
  virtual void beep() = 0;
  virtual void beep(uint frequency, uint duration) = 0;
  virtual void doubleBeep() = 0;
  virtual void doubleBeep(uint frequency, uint duration,
                          uint pauseDuration) = 0;
  virtual void rapidBeep() = 0;
  virtual void rapidBeep(uint frequency, uint duration, uint interval) = 0;
  virtual void stop() = 0;
  virtual bool isBeeping() = 0;
  virtual void adjustVolume(float volume) = 0;

  // Used for increment/decrement OilChangeTracker values at the same time as
  // the rapid beep.
  virtual void setOnRapidBeepCallback(std::function<void()> callback) = 0;
};
#pragma once

#include "InteractionSettings.h"
#include "HardwareComponent.h"
#include <memory>

class IBuzzer : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent;

  virtual ~IBuzzer() = default;

  virtual void updateSettings(std::shared_ptr<InteractionSettings> &settings) = 0;
  virtual void beep() = 0;
  virtual void beep(uint frequency, uint duration) = 0;
  virtual void doubleBeep() = 0;
  virtual void doubleBeep(uint frequency, uint duration,
                          uint pauseDuration) = 0;
  virtual void rapidBeep() = 0;
  virtual void rapidBeep(uint frequency, uint duration, uint interval) = 0;
  virtual void stop() = 0;
  virtual bool isBeeping() const = 0;
  virtual void setVolume(float volume) = 0;
};
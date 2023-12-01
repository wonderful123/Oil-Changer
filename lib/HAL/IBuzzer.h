#pragma once

#include "HardwareComponent.h"
#include "HardwarePinConfig.h"
#include "InteractionSettings.h"

class IBuzzer : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent;

  virtual ~IBuzzer() = default;

  virtual void updateSettings(const InteractionSettings &settings) = 0;
  virtual void beep(int frequency, int duration) = 0;
  virtual void stop() = 0;
  virtual bool isBeeping() const = 0;
  virtual void setVolume(float volume) = 0;

protected:
  InteractionSettings currentSettings;
};
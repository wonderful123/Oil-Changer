#pragma once

#include "HardwareComponent.h"
#include "HardwarePinConfig.h"
#include <functional>

/**
 * @class IButton
 * @brief Interface for Button implementations.
 *
 * This class defines a Button interface that should be implemented by
 * platform-specific classes.
 */
class IButton : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent; // Inherit constructor
  
  virtual ~IButton() = default; // Virtual destructor for proper cleanup

  /**
   * @brief Check if the button is currently pressed.
   *
   * @return True if pressed, false otherwise.
   */
  virtual bool isPressed() const = 0;

  // using ButtonPressCallback = std::function<void(const std::string &)>;
  using ButtonPressCallback = void (*)(const std::string &);
  virtual void setOnPressCallback(ButtonPressCallback callback) = 0;
};

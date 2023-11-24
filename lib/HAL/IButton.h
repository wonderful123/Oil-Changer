#pragma once

#include "HardwarePinConfig.h"
#include "HardwareComponent.h"

/**
 * @class IButton
 * @brief Interface for Button implementations.
 *
 * This class defines a Button interface that should be implemented by
 * platform-specific classes.
 */
class IButton : public HardwareComponent {
public:
  virtual ~IButton() = default; // Virtual destructor for proper cleanup

  /**
   * @brief Check if the button is currently pressed.
   *
   * @return True if pressed, false otherwise.
   */
  virtual bool isPressed() const = 0;

  using ButtonPressCallback = std::function<void(const std::string &)>;

  virtual void setOnPressCallback(ButtonPressCallback callback) = 0;

protected:
  explicit IButton(const HardwarePinConfig &config) : HardwareComponent(config) {}
};

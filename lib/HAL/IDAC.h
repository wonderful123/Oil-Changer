#pragma once

#include "HardwarePinConfig.h"
#include "HardwareComponent.h"

/**
 * @class IDAC
 * @brief Interface for DAC implementations.
 *
 * This class defines a DAC interface that should be implemented by
 * platform-specific classes.
 */
class IDAC : public HardwareComponent {
public:
  virtual ~IDAC() = default; // Virtual destructor for proper cleanup

  /**
   * @brief Set the output value of the DAC.
   *
   * @param value The value to be set (range and scale may be hardware
   * specific).
   */
  virtual void setValue(int value) = 0;

  /**
   * @brief Get the current output value of the DAC.
   *
   * @return Current DAC value.
   */
  virtual int getValue() const = 0;

protected:
  explicit IDAC(const HardwarePinConfig &config) : HardwareComponent(config) {}
};

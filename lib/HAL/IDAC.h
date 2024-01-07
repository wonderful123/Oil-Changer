// IDAC.h
#pragma once

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
  using HardwareComponent::HardwareComponent; // Inherit constructor
  
  virtual ~IDAC() = default; // Virtual destructor for proper cleanup

  /**
   * @brief Set the output value of the DAC.
   *
   * @param value The value to be set between 0 and 1
   */
  virtual void setValue(float value) = 0;

  /**
   * @brief Get the current output value of the DAC.
   *
   * @return Current DAC value.
   */
  virtual float getValue() const = 0;
};

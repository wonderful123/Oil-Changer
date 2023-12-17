#pragma once

#include "HardwareComponent.h"

/**
 * @class IADC
 * @brief Interface for ADC implementations.
 *
 * This class defines an ADC interface that should be implemented by
 * platform-specific classes.
 */
class IADC : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent; // Inherit constructor
  
  virtual ~IADC() = default; // virtual destructor to ensure proper cleanup for
                             // derived classes
  /**
   * @brief Reads the ADC value.
   *
   * @return ADC reading.
   */
  virtual int read() const = 0;

  /**
   * @brief Returns the ADC resolution.
   *
   * @return ADC resolution in bits.
   */
  virtual int resolution() const = 0;

protected:
  unsigned int _resolution;
};

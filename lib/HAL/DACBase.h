// DACBase.h
#pragma once

#include "IDAC.h"

/**
 * @class DACBase
 * @brief Base class for DAC implementations.
 *
 * Provides common functionality for DAC components.
 */
class DACBase : public IDAC {
public:
  explicit DACBase(const HardwarePinConfig &config) : IDAC(config) {
    setValue(0);
  }

  // Set the output value of the DAC
  void setValue(float value) override {
    // Clamp value between 0 and 1
    if (value < 0) {
      value = 0;
    } else if (value > 1) {
      value = 1;
    }
    _value = value;
    applyValue(_value);
  }

  // Get the current output value of the DAC
  float getValue() const override { return _value; }

protected:
  /**
   * @brief Apply the specified value to the DAC output.
   *
   * @param value The value to be applied between 0 and 1
   */
  virtual void applyValue(float value) = 0;

private:
  float _value; // Current value of the DAC
};

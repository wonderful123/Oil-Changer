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
  explicit DACBase(const HardwarePinConfig &config) : IDAC(config), _value(0) {}

  // Set the output value of the DAC
  void setValue(int value) override {
    _value = value;
    applyValue(_value);
  }

  // Get the current output value of the DAC
  int getValue() const override { return _value; }

protected:
  /**
   * @brief Apply the specified value to the DAC output.
   *
   * This method should be implemented in derived classes to
   * apply the value to the actual hardware.
   *
   * @param value The value to be applied.
   */
  virtual void applyValue(int value) = 0;

private:
  int _value; // Current value of the DAC
};

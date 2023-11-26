#pragma once

#ifdef PLATFORM_NATIVE

#include "PWMBase.h"
#include <gmock/gmock.h>

class MockPWM : public PWMBase {
public:
  explicit MockPWM(const HardwarePinConfig &config) : PWMBase(config) {}

  MOCK_METHOD(void, setDutyCycle, (double dutyCycle), (override));
  MOCK_METHOD(double, getDutyCycle, (), (const, override));
  MOCK_METHOD(void, setFrequency, (double frequency), (override));
  MOCK_METHOD(double, getFrequency, (), (const, override));

  // Mock methods for the protected virtual methods
  MOCK_METHOD(void, applyDutyCycle, (double dutyCycle), (override));
  MOCK_METHOD(void, applyFrequency, (double frequency), (override));
};

#endif // PLATFORM_NATIVE

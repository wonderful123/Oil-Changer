#pragma once

#include "IFlowMeter.h"

#ifdef PLATFORM_NATIVE

#include <gmock/gmock.h>

class MockFlowMeter : public IFlowMeter {
public:
  explicit MockFlowMeter(const HardwarePinConfig &config);

  MOCK_METHOD(double, getFlowRate, (), (const, override));
  MOCK_METHOD(double, getTotalVolume, (), (const, override));
  MOCK_METHOD(void, reset, (), (override));
};

#endif // PLATFORM_NATIVE
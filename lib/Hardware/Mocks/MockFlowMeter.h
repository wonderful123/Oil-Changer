#pragma once

#ifdef PLATFORM_NATIVE

#include "FlowMeterBase.h"
#include <gmock/gmock.h>

class MockFlowMeter : public FlowMeterBase {
public:
  explicit MockFlowMeter(const HardwarePinConfig &config)
      : FlowMeterBase(config) {}

    MOCK_METHOD(double, getFlowRate, (), (const, override));
    MOCK_METHOD(double, getTotalVolume, (), (const, override));
    MOCK_METHOD(void, reset, (), (override));
  };

#endif // PLATFORM_NATIVE
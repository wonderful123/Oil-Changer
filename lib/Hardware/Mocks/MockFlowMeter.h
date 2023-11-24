#pragma once

#include "IFileHandler.h"

#ifdef PLATFORM_NATIVE

#include <gmock/gmock.h>

class MockFlowMeter : public IFlowMeter {
public:
  MOCK_CONST_FUNC(double, getFlowRate, (), (override));
  MOCK_CONST_FUNC(double, getTotalVolume, (), (override));
  MOCK_FUNC(void, reset, (), (override));
};

#endif // PLATFORM_NATIVE
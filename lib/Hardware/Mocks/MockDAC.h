#pragma once

#ifdef PLATFORM_NATIVE

#include "DACBase.h"
#include <gmock/gmock.h>

class MockDAC : public DACBase {
public:
  explicit MockDAC(const HardwarePinConfig &config) : DACBase(config) {
    setInitialized(true); // Set initialized flag to true
  }

  MOCK_METHOD(void, applyValue, (int value), (override));
};

#endif // PLATFORM_NATIVE
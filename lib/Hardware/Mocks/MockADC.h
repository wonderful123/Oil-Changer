#pragma once

#ifdef PLATFORM_NATIVE

#include "ADCBase.h"
#include <gmock/gmock.h>
class MockADC : public ADCBase {
public:
  explicit MockADC(const HardwarePinConfig &config) : ADCBase(config) {
    setInitialized(true); // Set initialized flag to true in constructor
  }
  MOCK_METHOD(int, read, (), (const, override));
  MOCK_METHOD(int, resolution, (), (const, override));
  MOCK_METHOD(bool, isInitialized, (), (const, override));
};

#endif // PLATFORM_NATIVE
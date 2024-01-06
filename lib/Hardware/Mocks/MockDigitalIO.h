// MockDigitalIO.h
#pragma once

#ifdef PLATFORM_NATIVE

#include "DigitalIOBase.h"
#include <gmock/gmock.h>

class MockDigitalIO : public DigitalIOBase {
public:
  explicit MockDigitalIO(const HardwarePinConfig &config)
      : DigitalIOBase(config) {
    setInitialized(true); // Set initialized flag to true
  }

  MOCK_METHOD(int, read, (), (const, override));
  MOCK_METHOD(void, write, (int value), (override));
};

#endif // PLATFORM_NATIVE
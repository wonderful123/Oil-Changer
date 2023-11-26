#pragma once

#ifdef PLATFORM_NATIVE

#include <HardwarePinConfig.h>
#include <gmock/gmock.h>
#include "ISPI.h"

class MockSPI : public ISPI {
public:
  explicit MockSPI(const HardwarePinConfig &config) : ISPI(config) {}
  
  MOCK_METHOD(void, begin, (), (override));
  MOCK_METHOD(void, end, (), (override));
  MOCK_METHOD(uint8_t, transfer, (uint8_t data), (override));
  MOCK_METHOD(void, transfer, (void *buf, size_t count), (override));
};

#endif // PLATFORM_NATIVE
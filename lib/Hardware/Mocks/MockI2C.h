#pragma once

#ifdef PLATFORM_NATIVE

#include <cstddef>
#include <cstdint>
#include <gmock/gmock.h>
#include "II2C.h"

class MockI2C : public II2C {
public:
  MOCK_METHOD(void, begin, (), (override));
  MOCK_METHOD(void, end, (), (override));
  MOCK_METHOD(void, beginTransmission, (uint8_t address), (override));
  MOCK_METHOD(uint8_t, endTransmission, (bool sendStop), (override));
  MOCK_METHOD(size_t, write, (uint8_t value), (override));
  MOCK_METHOD(int, read, (), (override));
  MOCK_METHOD(int, available, (), (override));
};

#endif // PLATFORM_NATIVE
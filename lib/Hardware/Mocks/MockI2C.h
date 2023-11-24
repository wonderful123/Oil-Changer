#pragma once

#ifdef PLATFORM_NATIVE

#include <cstddef>
#include <cstdint>
#include <gmock/gmock.h>

class MockI2C : public II2C {
public:
  MOCK_FUNC(void, begin, (), (override));
  MOCK_FUNC(void, end, (), (override));
  MOCK_FUNC(void, beginTransmission, (uint8_t address), (override));
  MOCK_FUNC(uint8_t, endTransmission, (bool sendStop), (override));
  MOCK_FUNC(size_t, write, (uint8_t value), (override));
  MOCK_FUNC(int, read, (), (override));
  MOCK_FUNC(int, available, (), (override));
};

#endif // PLATFORM_NATIVE
#pragma once

#ifdef PLATFORM_NATIVE

#include <gmock/gmock.h>
#include <string>
#include "ISerial.h"

class MockSerial : public ISerial {
public:
  explicit MockSerial(const HardwarePinConfig &config) : ISerial(config) {}

  MOCK_METHOD(void, begin, (unsigned long baudrate), (override));
  MOCK_METHOD(void, end, (), (override));
  MOCK_METHOD(size_t, write, (uint8_t byte), (override));
  MOCK_METHOD(int, read, (), (override));
  MOCK_METHOD(int, available, (), (override));
  MOCK_METHOD(void, flush, (), (override));
};

#endif // PLATFORM_NATIVE
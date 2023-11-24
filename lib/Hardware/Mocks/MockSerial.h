#pragma once

#ifdef PLATFORM_NATIVE

#include <gmock/gmock.h>
#include <string>

class MockSerial : public ISerial {
public:
  MOCK_FUNC(void, begin, (unsigned long baudrate), (override));
  MOCK_FUNC(void, end, (), (override));
  MOCK_FUNC(size_t, write, (uint8_t byte), (override));
  MOCK_FUNC(int, read, (), (override));
  MOCK_FUNC(int, available, (), (override));
  MOCK_FUNC(void, flush, (), (override));
};

#endif // PLATFORM_NATIVE
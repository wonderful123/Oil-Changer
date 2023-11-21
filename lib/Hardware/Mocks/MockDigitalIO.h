#pragma once

#include "DigitalIOBase.h"
#include <gmock/gmock.h>

class MockDigitalIO : public DigitalIOBase {
public:
  explicit MockDigitalIO(const GpioPinConfig &config) : DigitalIOBase(config) {}

  MOCK_METHOD(int, read, (), (const, override));
  MOCK_METHOD(void, write, (int value), (override));
};
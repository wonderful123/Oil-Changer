#pragma once

#include "DACBase.h"
#include <gmock/gmock.h>

class MockDAC : public DACBase {
public:
  explicit MockDAC(const HardwarePinConfig &config) : DACBase(config) {}

  MOCK_METHOD(void, applyValue, (int value), (override));
};
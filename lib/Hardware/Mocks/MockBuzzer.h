#pragma once

#ifdef PLATFORM_NATIVE

#include "IBuzzer.h"
#include <gmock/gmock.h>

class MockBuzzer : public IBuzzer {
public:
  explicit MockBuzzer(const HardwarePinConfig &config) : IBuzzer(config) {}

  MOCK_METHOD(void, beep, (int frequency, int duration), (override));
  MOCK_METHOD(void, stop, (), (override));
  MOCK_METHOD(bool, isBeeping, (), (const, override));
  MOCK_METHOD(void, setVolume, (float volume), (override));
};

#endif // PLATFORM_NATIVE

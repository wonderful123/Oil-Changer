// MockHardwareConfig.h
#pragma once

#ifdef PLATFORM_NATIVE

#include <gmock/gmock.h>
#include "HardwareConfig.h"
#include "DisplayConfig.h"
#include <vector>

class MockHardwareConfig : public HardwareConfig {
public:
  MOCK_METHOD(const std::vector<HardwarePinConfig> &, getHardwarePinConfigs, (),
              (const));
  MOCK_METHOD(const std::vector<DisplayConfig> &, getDisplayConfigs, (),
              (const));
};

#endif // PLATFORM_NATIVE
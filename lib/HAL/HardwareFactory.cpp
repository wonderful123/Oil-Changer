#include "HardwareFactory.h"
#ifdef PLATFORM_ESP32
#include "ESP32/ESP32HardwareFactory.h"
#else
#include "MockHardwareFactory.h"
#endif

std::unique_ptr<HardwareFactory> &getHardwareFactory() {
  static std::unique_ptr<HardwareFactory> hardwareFactory;

  if (!hardwareFactory) {
#ifdef PLATFORM_ESP32
    hardwareFactory = std::make_unique<ESP32HardwareFactory>();
#else
    hardwareFactory = std::make_unique<MockHardwareFactory>();
#endif
  }

  return hardwareFactory;
}
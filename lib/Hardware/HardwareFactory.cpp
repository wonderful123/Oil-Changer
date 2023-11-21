#include "HardwareFactory.h"
#ifdef PLATFORM_ESP32
#include "ESP32HardwareFactory.h"
#else
#include <MockHardwareFactory.h>
#endif

std::unique_ptr<HardwareFactory> &getHardwareFactory() {
  static std::unique_ptr<HardwareFactory> hardwareFactory;

  if (!hardwareFactory) {
#ifdef PLATFORM_ESP32
    hardwareFactory.reset(new ESP32HardwareFactory());
#else
    hardwareFactory.reset(new MockHardwareFactory());
#endif
  }

  return hardwareFactory;
}
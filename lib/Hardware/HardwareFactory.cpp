#include "HardwareFactory.h"

#ifdef PLATFORM_ESP32
#include "ESP32HardwareFactory.h"
#else
#include <MockHardwareFactory.h>
#endif

std::unique_ptr<HardwareComponent>
HardwareFactory::createComponent(const HardwarePinConfig &config) {
  std::unique_ptr<HardwareComponent> hardwareComponent;

  if (config.type == "ADC") {
    hardwareComponent = createADC(config);
  } else if (config.type == "Button") {
    hardwareComponent = createButton(config);
  } else if (config.type == "DAC") {
    hardwareComponent = createDAC(config);
  } else if (config.type == "DigitalIO") {
    hardwareComponent = createDigitalIO(config);
  } else if (config.type == "FlowMeter") {
    hardwareComponent = createFlowMeter(config);
  } else if (config.type == "PWM") {
    hardwareComponent = createPWM(config);
  } else if (config.type == "Buzzer") {
    hardwareComponent = createBuzzer(config);
  } else if (config.type == "SPI") {
    hardwareComponent = createSPI(config);
  } else if (config.type == "I2C") {
    hardwareComponent = createI2C(config);
  } else if (config.type == "Serial") {
    hardwareComponent = createSerial(config);
  } else {
    Logger::error("Unknown hardware component type in hardware factory: " +
                  config.type);
    return nullptr;
  }

  if (!hardwareComponent) {
    Logger::error("Failed to create hardware component in hardware factory: " +
                  config.type);
  }

  return hardwareComponent;
}

std::shared_ptr<HardwareFactory> &HardwareFactory::getHardwareFactory() {
  static std::shared_ptr<HardwareFactory> hardwareFactory;
#ifdef PLATFORM_ESP32
  if (!hardwareFactory)
    hardwareFactory.reset(new ESP32HardwareFactory());
#else
  if (!hardwareFactory)
    hardwareFactory.reset(new MockHardwareFactory());
#endif
  return hardwareFactory;
}
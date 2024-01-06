// HardwareFactory.cpp

#include "HardwareFactory.h"

#include "HardwarePinConfig.h"
#include "IADC.h"
#include "IButton.h"
#include "IBuzzer.h"
#include "IDAC.h"
#include "IDigitalIO.h"
#include "IDisplay.h"
#include "IFlowMeter.h"
#include "II2C.h"
#include "IPWM.h"
#include "ISPI.h"
#include "ISerial.h"
#include "Logger.h"

#ifdef PLATFORM_ESP32
#include "ESP32HardwareFactory.h"
#else
#include "MockHardwareFactory.h"
#endif

std::shared_ptr<HardwareComponent> HardwareFactory::createComponent(
    const HardwarePinConfig &config) {
  std::shared_ptr<HardwareComponent> hardwareComponent;

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

std::shared_ptr<HardwareFactory> HardwareFactory::hardwareFactory = nullptr;
std::mutex HardwareFactory::mutex;

std::shared_ptr<HardwareFactory> HardwareFactory::createInstance() {
#ifdef PLATFORM_ESP32
  return std::make_shared<ESP32HardwareFactory>();
#else
  return std::make_shared<MockHardwareFactory>();
#endif
}

std::shared_ptr<HardwareFactory> &HardwareFactory::getHardwareFactory() {
  std::lock_guard<std::mutex> lock(mutex);
  if (!hardwareFactory) {
    hardwareFactory = createInstance();
  }
  return hardwareFactory;
}
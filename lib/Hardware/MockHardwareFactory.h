#pragma once

#ifdef PLATFORM_NATIVE

#include "HardwareFactory.h"
#include "Mocks/MockADC.h"
#include "Mocks/MockButton.h"
#include "Mocks/MockBuzzer.h"
#include "Mocks/MockDAC.h"
#include "Mocks/MockDigitalIO.h"
#include "Mocks/MockFileHandler.h"
#include "Mocks/MockFlowMeter.h"
#include "Mocks/MockI2C.h"
#include "Mocks/MockPWM.h"
#include "Mocks/MockSPI.h"
#include "Mocks/MockSerial.h"
#include <gmock/gmock.h>

class MockHardwareFactory : public HardwareFactory {
public:
  // Mock methods for each type of component creation
  MOCK_METHOD(std::unique_ptr<IADC>, createADC, (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<IButton>, createButton,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<IDAC>, createDAC, (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<IDigitalIO>, createDigitalIO,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<IFlowMeter>, createFlowMeter,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<IPWM>, createPWM, (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<IBuzzer>, createBuzzer,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<ISPI>, createSPI, (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<II2C>, createI2C, (const HardwarePinConfig &));
  MOCK_METHOD(std::unique_ptr<ISerial>, createSerial,
              (const HardwarePinConfig &));

  std::unique_ptr<IFileHandler> createFileHandler() {
    return std::unique_ptr<IFileHandler>(new MockFileHandler());
  }

  std::shared_ptr<HardwareComponent>
  createComponent(const HardwarePinConfig &config) override {
    if (config.type == "ADC") {
      return createADC(config);
    } else if (config.type == "Button") {
      return createButton(config);
    } else if (config.type == "DAC") {
      return createDAC(config);
    } else if (config.type == "DigitalIO") {
      return createDigitalIO(config);
    } else if (config.type == "FlowMeter") {
      return createFlowMeter(config);
    } else if (config.type == "PWM") {
      return createPWM(config);
    } else if (config.type == "Buzzer") {
      return createBuzzer(config);
    } else if (config.type == "SPI") {
      return createSPI(config);
    } else if (config.type == "I2C") {
      return createI2C(config);
    } else if (config.type == "Serial") {
      return createSerial(config);
    } else {
      Logger::error(
          "Unknown hardware component type in mock hardware factory: " +
          config.type);
      return nullptr;
    }
  }
};

#endif
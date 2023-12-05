#pragma once

#ifdef PLATFORM_ESP32

#include "ESP32/ESP32ADC.h"
#include "ESP32/ESP32Button.h"
#include "ESP32/ESP32Buzzer.h"
#include "ESP32/ESP32DAC.h"
#include "ESP32/ESP32DigitalIO.h"
#include "ESP32/ESP32FileHandler.h"
#include "ESP32/ESP32FlowMeter.h"
#include "ESP32/ESP32I2C.h"
#include "ESP32/ESP32InterfaceDisplay.h"
#include "ESP32/ESP32PWM.h"
#include "ESP32/ESP32SPI.h"
#include "ESP32/ESP32Serial.h"
#include "HardwareFactory.h"
#include <memory>

class ESP32HardwareFactory : public HardwareFactory {
public:
  virtual std::unique_ptr<IADC>
  createADC(const HardwarePinConfig &config) override {
    return std::unique_ptr<IADC>(new ESP32ADC(config));
  }
  virtual std::unique_ptr<IButton>
  createButton(const HardwarePinConfig &config) override {
    return std::unique_ptr<IButton>(new ESP32Button(config));
  }
  virtual std::unique_ptr<IDAC>
  createDAC(const HardwarePinConfig &config) override {
    return std::unique_ptr<IDAC>(new ESP32DAC(config));
  }
  virtual std::unique_ptr<IDigitalIO>
  createDigitalIO(const HardwarePinConfig &config) override {
    return std::unique_ptr<IDigitalIO>(new ESP32DigitalIO(config));
  }
  virtual std::unique_ptr<IFlowMeter>
  createFlowMeter(const HardwarePinConfig &config) override {
    return std::unique_ptr<IFlowMeter>(new ESP32FlowMeter(config));
  }
  virtual std::unique_ptr<IPWM>
  createPWM(const HardwarePinConfig &config) override {
    return std::unique_ptr<IPWM>(new ESP32PWM(config));
  }
  virtual std::unique_ptr<IBuzzer>
  createBuzzer(const HardwarePinConfig &config) override {
    return std::unique_ptr<IBuzzer>(new ESP32Buzzer(config));
  }
  virtual std::unique_ptr<ISPI>
  createSPI(const HardwarePinConfig &config) override {
    return std::unique_ptr<ISPI>(new ESP32SPI(config));
  }
  virtual std::unique_ptr<II2C>
  createI2C(const HardwarePinConfig &config) override {
    return std::unique_ptr<II2C>(new ESP32I2C(config));
  }
  virtual std::unique_ptr<ISerial>
  createSerial(const HardwarePinConfig &config) override {
    return std::unique_ptr<ISerial>(new ESP32Serial(config));
  }
  virtual std::unique_ptr<IFileHandler> createFileHandler() override {
    return std::unique_ptr<IFileHandler>(new ESP32FileHandler());
  }
  virtual std::unique_ptr<IDisplay> createDisplay(const std::string &displayId, ICommunicationInterface &commInterface) override {
    return std::make_unique<ESP32InterfaceDisplay>(commInterface, displayId);
  }

  virtual std::unique_ptr<ICommunicationInterface> createCommunicationInterface() override {
    // Example implementation assuming a serial communication interface
    return std::make_unique<ESP32SerialCommunication>();
  }
};

#endif
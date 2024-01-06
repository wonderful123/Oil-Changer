// ESP32HardwareFactory.h
#pragma once

#ifdef PLATFORM_ESP32

#include <memory>

#include "ESP32/ESP32ADC.h"
#include "ESP32/ESP32Button.h"
#include "ESP32/ESP32Buzzer.h"
#include "ESP32/ESP32DAC.h"
#include "ESP32/ESP32DigitalIO.h"
#include "ESP32/ESP32FileHandler.h"
#include "ESP32/ESP32FlowMeter.h"
#include "ESP32/ESP32I2C.h"
#include "ESP32/ESP32PWM.h"
#include "ESP32/ESP32SPI.h"
#include "ESP32/ESP32Serial.h"
#include "ESP32/ESP32SerialDisplay.h"
#include "HardwareFactory.h"

class ESP32HardwareFactory : public HardwareFactory {
 public:
  virtual std::shared_ptr<IADC> createADC(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<IADC>(new ESP32ADC(config));
  }
  virtual std::shared_ptr<IButton> createButton(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<IButton>(new ESP32Button(config));
  }
  virtual std::shared_ptr<IDAC> createDAC(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<IDAC>(new ESP32DAC(config));
  }
  virtual std::shared_ptr<IDigitalIO> createDigitalIO(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<IDigitalIO>(new ESP32DigitalIO(config));
  }
  virtual std::shared_ptr<IFlowMeter> createFlowMeter(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<IFlowMeter>(new ESP32FlowMeter(config));
  }
  virtual std::shared_ptr<IPWM> createPWM(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<IPWM>(new ESP32PWM(config));
  }
  virtual std::shared_ptr<IBuzzer> createBuzzer(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<IBuzzer>(new ESP32Buzzer(config));
  }
  virtual std::shared_ptr<ISPI> createSPI(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<ISPI>(new ESP32SPI(config));
  }
  virtual std::shared_ptr<II2C> createI2C(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<II2C>(new ESP32I2C(config));
  }
  virtual std::shared_ptr<ISerial> createSerial(
      const HardwarePinConfig &config) override {
    return std::shared_ptr<ISerial>(new ESP32Serial(config));
  }
  virtual std::shared_ptr<IFileHandler> createFileHandler() override {
    return std::shared_ptr<IFileHandler>(new ESP32FileHandler());
  }
  std::shared_ptr<IDisplay> createDisplay(
      const std::string &displayId, ICommunicationInterface &commInterface,
      const HardwarePinConfig &config) override {
    return std::make_shared<ESP32SerialDisplay>(displayId, commInterface,
                                                config);
  }
};

#endif
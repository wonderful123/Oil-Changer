#pragma once

#include "ESP32/ADC.h"
#include "ESP32/Button.h"
#include "ESP32/Buzzer.h"
#include "ESP32/DAC.h"
#include "ESP32/DigitalIO.h"
#include "ESP32/FileHandler.h"
#include "ESP32/FlowMeter.h"
#include "ESP32/PWM.h"
#include "HardwareFactory.h"
#include <memory>

class ESP32HardwareFactory : public HardwareFactory {
public:
  virtual std::unique_ptr<IADC>
  createADC(const GpioPinConfig &config) override {
    return std::unique_ptr<IADC>(new ADC(config));
  }
  virtual std::unique_ptr<IButton>
  createButton(const GpioPinConfig &config) override {
    return std::unique_ptr<IButton>(new Button(config));
  }
  virtual std::unique_ptr<IDAC>
  createDAC(const GpioPinConfig &config) override {
    return std::unique_ptr<IDAC>(new DAC(config));
  }
  virtual std::unique_ptr<IDigitalIO>
  createDigitalIO(const GpioPinConfig &config) override {
    return std::unique_ptr<IDigitalIO>(new DigitalIO(config));
  }
  std::unique_ptr<IFlowMeter> createFlowMeter(const GpioPinConfig &config) {
    return std::unique_ptr<IFlowMeter>(new FlowMeter(config));
  }
  virtual std::unique_ptr<IPWM>
  createPWM(const GpioPinConfig &config) override {
    return std::unique_ptr<IPWM>(new PWM(config));
  }
  virtual std::unique_ptr<IBuzzer>
  createBuzzer(const GpioPinConfig &config) override {
    return std::unique_ptr<IBuzzer>(new Buzzer(config));
  }
  virtual std::unique_ptr<ISPI>
  createSPI(const GpioPinConfig &config) override {
    return std::unique_ptr<ISPI>(new ESP32SPI(config));
  }

  virtual std::unique_ptr<II2C>
  createI2C(const GpioPinConfig &config) override {
    return std::unique_ptr<II2C>(new ESP32I2C(config));
  }

  virtual std::unique_ptr<ISerial>
  createSerial(const GpioPinConfig &config) override {
    return std::unique_ptr<ISerial>(new ESP32Serial(config));
  }
  
  virtual std::unique_ptr<IFileHandler> createFileHandler() override {
    return std::unique_ptr<IFileHandler>(new FileHandler());
  }
};

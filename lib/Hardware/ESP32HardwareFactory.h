// ESP32HardwareFactory.h
#pragma once

#ifdef PLATFORM_ESP32

#include <memory>

#include "DisplayConfig.h"
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
  virtual std::shared_ptr<IADC>
  createADC(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32ADC>(config);
  }
  virtual std::shared_ptr<IButton>
  createButton(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32Button>(config);
  }
  virtual std::shared_ptr<IDAC>
  createDAC(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32DAC>(config);
  }
  virtual std::shared_ptr<IDigitalIO>
  createDigitalIO(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32DigitalIO>(config);
  }
  virtual std::shared_ptr<IFlowMeter>
  createFlowMeter(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32FlowMeter>(config);
  }
  virtual std::shared_ptr<IPWM>
  createPWM(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32PWM>(config);
  }
  virtual std::shared_ptr<IBuzzer>
  createBuzzer(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32Buzzer>(config);
  }
  virtual std::shared_ptr<ISPI>
  createSPI(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32SPI>(config);
  }
  virtual std::shared_ptr<II2C>
  createI2C(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32I2C>(config);
  }
  virtual std::shared_ptr<ISerial>
  createSerial(const HardwarePinConfig &config) override {
    return std::make_shared<ESP32Serial>(config);
  }
  virtual std::shared_ptr<IFileHandler> createFileHandler() override {
    return std::make_shared<ESP32FileHandler>();
  }
  virtual std::shared_ptr<IDisplay>
  createDisplay(const DisplayConfig &config,
                std::shared_ptr<ICommunicationInterface> commInterface) {
    std::shared_ptr<ISerial> serial =
        std::static_pointer_cast<ISerial>(commInterface);
    return std::make_shared<ESP32SerialDisplay>(serial, config.id);
  }
};

#endif
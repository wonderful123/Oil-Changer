#pragma once

#include "ESP32/ADC.h"
#include "ESP32/Buzzer.h"
#include "ESP32/DigitalIO.h"
#include "ESP32/FileHandler.h"
#include "ESP32/PWM.h"
#include "HardwareFactory.h"
#include <memory>

class ESP32HardwareFactory : public HardwareFactory {
public:
  virtual std::unique_ptr<IADC>
  createADC(const GpioPinConfig &config) override {
    return std::unique_ptr<IADC>(new ADC(config));
  }

  virtual std::unique_ptr<IDigitalIO>
  createDigitalIO(const GpioPinConfig &config) override {
    return std::unique_ptr<IDigitalIO>(new DigitalIO(config));
  }

  virtual std::unique_ptr<IPWM>
  createPWM(const GpioPinConfig &config) override {
    return std::unique_ptr<IPWM>(new PWM(config));
  }

  virtual std::unique_ptr<IBuzzer>
  createBuzzer(const GpioPinConfig &config) override {
    return std::unique_ptr<IBuzzer>(new Buzzer(config));
  }

  virtual std::unique_ptr<IFileHandler> createFileHandler() override {
    return std::unique_ptr<IFileHandler>(new FileHandler());
  }
};

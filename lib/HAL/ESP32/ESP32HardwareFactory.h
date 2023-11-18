// ESP32HardwareFactory.h
#pragma once

#include "ESP32/ADC.h"
#include "ESP32/Buzzer.h"
#include "ESP32/DigitalIO.h"
#include "ESP32/FileHandler.h"
#include "ESP32/PWM.h"
#include "HardwareFactory.h"

class ESP32HardwareFactory : public HardwareFactory {
public:
  std::unique_ptr<ADC> createADC(const GpioPinConfig &config) override {
    return std::make_unique<ESP32::ADC>(config);
  }

  std::unique_ptr<DigitalIO>
  createDigitalIO(const GpioPinConfig &config) override {
    return std::make_unique<ESP32::DigitalIO>(config);
  }

  std::unique_ptr<PWM> createPWM(const GpioPinConfig &config) override {
    return std::make_unique<ESP32::PWM>(config);
  }

  std::unique_ptr<Buzzer> createBuzzer(const GpioPinConfig &config) override {
    return std::make_unique<ESP32::Buzzer>(config);
  }

  std::unique_ptr<IFileHandler> createFileHandler() override {
    return std::make_unique<ESP32::FileHandler>();
  }
};

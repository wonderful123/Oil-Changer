#pragma once

#include "GpioPinConfig.h"
#include "IADC.h"
#include "IBuzzer.h"
#include "IDigitalIO.h"
#include "IFileHandler.h"
#include "IPWM.h"
#include <memory>

class HardwareFactory {
public:
  virtual ~HardwareFactory() = default;

  // Factory getter function declaration
  std::unique_ptr<HardwareFactory> &getHardwareFactory();

  virtual std::unique_ptr<IADC> createADC(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IDigitalIO>
  createDigitalIO(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IPWM> createPWM(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IBuzzer>
  createBuzzer(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IFileHandler> createFileHandler() = 0;
};

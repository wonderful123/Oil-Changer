#pragma once

#include "GpioPinConfig.h"
#include "HardwareComponent.h"
#include "IADC.h"
#include "IButton.h"
#include "IBuzzer.h"
#include "IDAC.h"
#include "IDigitalIO.h"
#include "IPWM.h"
#include "IFileHandler.h"
#include <memory>

class HardwareFactory {
public:
  virtual ~HardwareFactory() = default;

  // Singleton getter function
  static std::unique_ptr<HardwareFactory> &getHardwareFactory();

  std::unique_ptr<HardwareComponent> createComponent(const GpioPinConfig &config);

  // Factory methods for creating hardware components
  virtual std::unique_ptr<IADC> createADC(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IButton> createButton(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IDAC> createDAC(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IDigitalIO> createDigitalIO(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IPWM> createPWM(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IBuzzer> createBuzzer(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IFileHandler> createFileHandler() = 0;

private:
  std::unique_ptr<HardwareFactory> _hardwareFactory;
};

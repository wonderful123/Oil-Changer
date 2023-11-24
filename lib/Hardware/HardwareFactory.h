#pragma once

#include "GpioPinConfig.h"
#include "HardwareComponent.h"
#include "IADC.h"
#include "IButton.h"
#include "IBuzzer.h"
#include "IDAC.h"
#include "IDigitalIO.h"
#include "IFileHandler.h"
#include "IFlowMeter.h"
#include "IPWM.h"
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
  virtual std::unique_ptr<IFlowMeter> createFlowMeter(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IPWM> createPWM(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IBuzzer> createBuzzer(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<IFileHandler> createFileHandler() = 0;
  virtual std::unique_ptr<ISPI> createSPI(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<II2C> createI2C(const GpioPinConfig &config) = 0;
  virtual std::unique_ptr<ISerial> createSerial(const GpioPinConfig &config) = 0;
  
private:
  std::unique_ptr<HardwareFactory> _hardwareFactory;
};

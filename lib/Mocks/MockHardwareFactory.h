#pragma once

#include "HardwareFactory.h"
#include <MockADC.h>
#include "MockBuzzer.h"
#include "MockDigitalIO.h"
#include "MockFileHandler.h"
#include "MockPWM.h"
#include <gmock/gmock.h>

class MockHardwareFactory : public HardwareFactory {
public:
  std::unique_ptr<IADC> createADC(const GpioPinConfig &config) override {
    return std::unique_ptr<IADC>(new MockADC(config));
  }

  std::unique_ptr<IDigitalIO>
  createDigitalIO(const GpioPinConfig &config) override {
    return std::unique_ptr<IDigitalIO>(new MockDigitalIO(config));
  }

  std::unique_ptr<IPWM> createPWM(const GpioPinConfig &config) override {
    return std::unique_ptr<IPWM>(new MockPWM(config));
  }

  std::unique_ptr<IBuzzer> createBuzzer(const GpioPinConfig &config) override {
    return std::unique_ptr<IBuzzer>(new MockBuzzer(config));
  }

  std::unique_ptr<IFileHandler> createFileHandler() override {
    return std::unique_ptr<IFileHandler>(new MockFileHandler());
  }
};

#pragma once

#include "HardwareFactory.h"
#include "Mocks/MockADC.h"
#include "Mocks/MockDAC.h"
#include "Mocks/MockBuzzer.h"
#include "Mocks/MockDigitalIO.h"
#include "Mocks/MockFileHandler.h"
#include "Mocks/MockPWM.h"
#include "Mocks/MockButton.h"

class MockHardwareFactory : public HardwareFactory {
public:
  std::unique_ptr<IADC> createADC(const HardwarePinConfig &config) override {
    return std::unique_ptr<IADC>(new MockADC(config));
  }

  std::unique_ptr<IButton> createButton(const HardwarePinConfig &config) override {
    return std::unique_ptr<IButton>(new MockButton(config));
  }

  std::unique_ptr<IDAC> createDAC(const HardwarePinConfig &config) override {
    return std::unique_ptr<IDAC>(new MockDAC(config));
  }
  std::unique_ptr<IFlowMeter> createFlowMeter(const HardwarePinConfig &config) {
    return std::unique_ptr<IFlowMeter>(new FlowMeter(config));
  }
  std::unique_ptr<IDigitalIO>
  createDigitalIO(const HardwarePinConfig &config) override {
    return std::unique_ptr<IDigitalIO>(new MockDigitalIO(config));
  }

  std::unique_ptr<IPWM> createPWM(const HardwarePinConfig &config) override {
    return std::unique_ptr<IPWM>(new MockPWM(config));
  }

  std::unique_ptr<IBuzzer> createBuzzer(const HardwarePinConfig &config) override {
    return std::unique_ptr<IBuzzer>(new MockBuzzer(config));
  }

  std::unique_ptr<IFileHandler> createFileHandler() override {
    return std::unique_ptr<IFileHandler>(new MockFileHandler());
  }
};

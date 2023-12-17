#pragma once

#ifdef PLATFORM_NATIVE

#include "HardwareFactory.h"
#include "ICommunicationInterface.h"
#include "IFileHandler.h"
#include <gmock/gmock.h>

class MockHardwareFactory : public HardwareFactory {
public:
  // Mock methods for each type of component creation
  MOCK_METHOD(std::shared_ptr<IADC>, createADC, (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<IButton>, createButton,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<IDAC>, createDAC, (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<IDigitalIO>, createDigitalIO,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<IFlowMeter>, createFlowMeter,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<IPWM>, createPWM, (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<IBuzzer>, createBuzzer,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<ISPI>, createSPI, (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<II2C>, createI2C, (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<ISerial>, createSerial,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<HardwareComponent>, createComponent,
              (const HardwarePinConfig &));
  MOCK_METHOD(std::shared_ptr<IFileHandler>, createFileHandler, ());
  MOCK_METHOD(std::shared_ptr<ICommunicationInterface>,
              createCommunicationInterface, ());
  MOCK_METHOD(std::shared_ptr<IDisplay>, createDisplay,
              (const std::string &displayId,
               ICommunicationInterface &commInterface));
};

#endif
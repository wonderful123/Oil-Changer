// HardwareFactory.h
#pragma once

#include <memory>
#include <mutex>

class HardwarePinConfig;
class DisplayConfig;
class HardwareComponent;
class IADC;
class IButton;
class IBuzzer;
class IDAC;
class IDigitalIO;
class IDisplay;
class ICommunicationInterface;
class IFileHandler;
class IFlowMeter;
class II2C;
class IPWM;
class ISerial;
class ISPI;

class HardwareFactory {
public:
  virtual ~HardwareFactory() = default;

  // Singleton getter function
  static std::shared_ptr<HardwareFactory> &getHardwareFactory();
  static void resetInstance() { getHardwareFactory().reset(); }
  static std::shared_ptr<HardwareFactory> createInstance();

  virtual std::shared_ptr<HardwareComponent>
  createComponent(const HardwarePinConfig &config);

  // Factory methods for creating hardware components
  virtual std::shared_ptr<IADC> createADC(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<IButton>
  createButton(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<IDAC> createDAC(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<IDigitalIO>
  createDigitalIO(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<IFlowMeter>
  createFlowMeter(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<IPWM> createPWM(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<IBuzzer>
  createBuzzer(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<IFileHandler> createFileHandler() = 0;
  virtual std::shared_ptr<ISPI> createSPI(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<II2C> createI2C(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<ISerial>
  createSerial(const HardwarePinConfig &config) = 0;
  virtual std::shared_ptr<IDisplay>
  createDisplay(const DisplayConfig &config,
                std::shared_ptr<ICommunicationInterface> commInterface) = 0;

private:
  static std::shared_ptr<HardwareFactory> hardwareFactory;
  static std::mutex mutex;
};

#pragma once

#include "HardwareComponent.h"
#include "HardwarePinConfig.h"
#include "IADC.h"
#include "IButton.h"
#include "IBuzzer.h"
#include "IDAC.h"
#include "IDigitalIO.h"
#include "IDisplay.h"
#include "ICommunicationInterface.h"
#include "IFileHandler.h"
#include "IFlowMeter.h"
#include "II2C.h"
#include "IPWM.h"
#include "ISPI.h"
#include "ISerial.h"
#include <memory>

class HardwareFactory {
public:
    virtual ~HardwareFactory() = default;

    // Singleton getter function
    static std::shared_ptr<HardwareFactory> &getHardwareFactory();

    virtual std::shared_ptr<HardwareComponent>
    createComponent(const HardwarePinConfig &config);

    // Factory methods for creating hardware components
    virtual std::unique_ptr<IADC> createADC(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<IButton> createButton(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<IDAC> createDAC(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<IDigitalIO> createDigitalIO(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<IFlowMeter> createFlowMeter(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<IPWM> createPWM(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<IBuzzer> createBuzzer(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<IFileHandler> createFileHandler() = 0;
    virtual std::unique_ptr<ISPI> createSPI(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<II2C> createI2C(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<ISerial> createSerial(const HardwarePinConfig &config) = 0;
    virtual std::unique_ptr<IDisplay> createDisplay(const std::string &displayId, ICommunicationInterface &commInterface) = 0;
    virtual std::unique_ptr<ICommunicationInterface> createCommunicationInterface() = 0;

private:
    std::shared_ptr<HardwareFactory> _hardwareFactory;
};

#include "HardwareManager.h"
#include "Logger.h"
#include <iostream>

void HardwareManager::initializeHardware() {
  auto hardwareConfig = _configManager->getHardwareConfig();
  if (hardwareConfig) {
    for (const auto &config : hardwareConfig->getGpioConfigs()) {
      std::cout << "Config: Pin Number = " << config.pinNumber
                << ", ID = " << config.id << std::endl;
      initializeComponent(config);
    }
  } else {
    Logger::error("Hardware configuration is not available.");
  }
}

void HardwareManager::initializeComponent(const GpioPinConfig &config) {
  if (config.type == "ADC") {
    adcs[config.pinNumber] = _hardwareFactory->createADC(config);
  } else if (config.type == "DigitalIO") {
    digitalIOs[config.pinNumber] = _hardwareFactory->createDigitalIO(config);
  } else if (config.type == "PWM") {
    pwms[config.pinNumber] = _hardwareFactory->createPWM(config);
  } else if (config.type == "Buzzer") {
    buzzer = _hardwareFactory->createBuzzer(config);
  }
  // Add more types as needed
}

bool HardwareManager::isComponentInitialized(const std::string &componentType) {
  if (componentType == "DigitalIO") {
    std::cout << "DigitalIOs size: " << digitalIOs.size() << std::endl;
    for (const auto &dio : digitalIOs) {
      std::cout << "isInitialized: " << dio.second->isInitialized() << "..."
                << std::endl;
      if (!dio.second->isInitialized()) {
        return false;
      }
    }
    return !digitalIOs.empty();
  } else if (componentType == "ADC") {
    for (const auto &adc : adcs) {
      if (!adc.second->isInitialized()) {
        return false;
      }
    }
    return !adcs.empty();
  } else if (componentType == "PWM") {
    for (const auto &pwm : pwms) {
      if (!pwm.second->isInitialized()) {
        return false;
      }
    }
    return !pwms.empty();
  } else if (componentType == "Buzzer") {
    return buzzer && buzzer->isInitialized();
  }
  return false;
}

void HardwareManager::update() {
  // This method is called when the observed subject changes.

  // Example implementation:
  // 1. Check the status of the observed subject.
  // 2. Based on the new status, take appropriate action.
  //    This might involve adjusting hardware settings, triggering events, etc.

  // Example: Check if a sensor value is out of range and respond accordingly
  // if (sensorManager.isSensorValueOutOfRange()) {
  //     handleSensorOutOfRange();
  // }

  // You can also log the update or perform other operations as needed.
  Logger::info("HardwareManager received an update notification.");

  // Implement specific logic based on your application's requirements.
}
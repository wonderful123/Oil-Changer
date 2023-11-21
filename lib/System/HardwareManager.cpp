#include "HardwareManager.h"
#include "Logger.h"
#include "FSM/StateMachine.h"

void HardwareManager::initializeHardware() {
  auto hardwareConfig = _configManager->getHardwareConfig();
  if (hardwareConfig) {
    bool allComponentsInitialized = true;
    for (const auto &config : hardwareConfig->getGpioConfigs()) {
      if (!initializeComponent(config)) {
        allComponentsInitialized = false;
        break; // Stop initialization if any component fails
      }
    }
    if (allComponentsInitialized) {
      Logger::info("Hardware initialization successful");
    } else {
      Logger::error("Some hardware components failed to initialize");
    }
  } else {
    Logger::error("Hardware configuration is not available");
  }
}
bool HardwareManager::initializeComponent(const GpioPinConfig &config) {
  if (config.type == "ADC") {
    auto adc = _hardwareFactory->createADC(config);
    if (!adc) {
      Logger::error("Failed to initialize ADC on pin " +
                    std::to_string(config.pinNumber));
      return false;
    }
    adcs[config.pinNumber] = std::move(adc);
  } else if (config.type == "DigitalIO") {
    auto digitalIO = _hardwareFactory->createDigitalIO(config);
    if (!digitalIO) {
      Logger::error("Failed to initialize DigitalIO on pin " +
                    std::to_string(config.pinNumber));
      return false;
    }
    digitalIOs[config.pinNumber] = std::move(digitalIO);
  } else if (config.type == "PWM") {
    auto pwm = _hardwareFactory->createPWM(config);
    if (!pwm) {
      Logger::error("Failed to initialize PWM on pin " +
                    std::to_string(config.pinNumber));
      return false;
    }
    pwms[config.pinNumber] = std::move(pwm);
  } else if (config.type == "Buzzer") {
    auto buzzerComponent = _hardwareFactory->createBuzzer(config);
    if (!buzzerComponent) {
      Logger::error("Failed to initialize Buzzer on pin " +
                    std::to_string(config.pinNumber));
      return false;
    }
    buzzer = std::move(buzzerComponent);
  } else {
    Logger::error("Unrecognized component type: " + config.type);
    return false;
  }

  // Log successful initialization
  Logger::info(config.type + " component initialized successfully on pin " +
               std::to_string(config.pinNumber));
  return true;
}

bool HardwareManager::isComponentInitialized(const std::string &componentType) {
  if (componentType == "DigitalIO") {
    for (const auto &dio : digitalIOs) {
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

void HardwareManager::handleButtonPress(int buttonId) {
  StateMachine stateMachine;
  ButtonPressEvent pressEvent(buttonId);
  stateMachine.handleEvent(pressEvent);
}
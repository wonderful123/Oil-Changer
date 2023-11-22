#include "HardwareManager.h"
#include "FSM/StateMachine.h"
#include "Logger.h"

HardwareManager::HardwareManager(
    std::shared_ptr<ConfigManager> configManager,
    std::unique_ptr<HardwareFactory> hardwareFactory,
    std::shared_ptr<ButtonController> buttonController)
    : _configManager(std::move(configManager)),
      _hardwareFactory(std::move(hardwareFactory)),
      _buttonController(std::move(buttonController)) {

  initializerMap["ADC"] = [this](const auto &config) { return initializeADC(config); };
  initializerMap["DAC"] = [this](const auto &config) { return initializeDAC(config); };
  initializerMap["DigitalIO"] = [this](const auto &config) { return initializeDigitalIO(config); };
  initializerMap["Button"] = [this](const auto &config) { return initializeButton(config); };
  initializerMap["PWM"] = [this](const auto &config) { return initializePWM(config); };
}

void HardwareManager::initializeHardware() {
  auto hardwareConfig = _configManager->getHardwareConfig();
  if (!hardwareConfig) {
    Logger::error("Hardware configuration is not available");
    return;
  }

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
}

bool HardwareManager::initializeComponent(const GpioPinConfig &config) {
  if (config.type == "Buzzer") {
    return initializeBuzzer(config);
  }

  auto initializer = initializerMap.find(config.type);
  if (initializer == initializerMap.end()) {
    Logger::error("Unrecognized component type: " + config.type);
    return false;
  }

  return initializer->second(config);
}

bool HardwareManager::isComponentInitialized(const std::string &componentType) {
  auto checkInitialized = [](const auto &components) {
    return std::all_of(components.begin(), components.end(),
                       [](const auto &component) {
                         return component.second->isInitialized();
                       }) &&
           !components.empty();
  };

  if (componentType == "DigitalIO") {
    return checkInitialized(digitalIOs);
  } else if (componentType == "ADC") {
    return checkInitialized(adcs);
  } else if (componentType == "DAC") {
    return checkInitialized(dacs);
  } else if (componentType == "PWM") {
    return checkInitialized(pwms);
  } else if (componentType == "Button") {
    return checkInitialized(buttons);
  } else if (componentType == "Buzzer") {
    return buzzer && buzzer->isInitialized();
  }

  return false;
}

void HardwareManager::update() {
  // This method is called when the observed subject changes.

  // You can also log the update or perform other operations as needed.
  Logger::info("HardwareManager received an update notification.");

  // Implement specific logic based on your application's requirements.
}

bool HardwareManager::initializeADC(const GpioPinConfig &config) {
  auto adc = _hardwareFactory->createADC(config);
  if (!adc) {
    Logger::error("Failed to initialize ADC on pin " +
                  std::to_string(config.pinNumber));
    return false;
  }
  adcs[config.pinNumber] = std::move(adc);
  return true;
}

bool HardwareManager::initializeDAC(const GpioPinConfig &config) {
  auto dac = _hardwareFactory->createDAC(config);
  if (!dac) {
    Logger::error("Failed to initialize DAC on pin " +
                  std::to_string(config.pinNumber));
    return false;
  }
  dacs[config.pinNumber] = std::move(dac);
  return true;
}

bool HardwareManager::initializeDigitalIO(const GpioPinConfig &config) {
  auto digitalIO = _hardwareFactory->createDigitalIO(config);
  if (!digitalIO) {
    Logger::error("Failed to initialize DigitalIO on pin " +
                  std::to_string(config.pinNumber));
    return false;
  }
  digitalIOs[config.pinNumber] = std::move(digitalIO);
  return true;
}

bool HardwareManager::initializeButton(const GpioPinConfig &config) {
  auto buttonUnique = _hardwareFactory->createButton(config);
  if (!buttonUnique) {
    Logger::error("Failed to initialize Button on pin " +
                  std::to_string(config.pinNumber));
    return false;
  }
  std::shared_ptr<IButton> buttonShared =
      std::move(buttonUnique); // Convert to shared_ptr
  _buttonController->registerButton(config.pinNumber, std::move(buttonShared));
  _buttonIdToPinMap[config.id] = config.pinNumber;
  return true;
}

bool HardwareManager::initializePWM(const GpioPinConfig &config) {
  auto pwm = _hardwareFactory->createPWM(config);
  if (!pwm) {
    Logger::error("Failed to initialize PWM on pin " +
                  std::to_string(config.pinNumber));
    return false;
  }
  pwms[config.pinNumber] = std::move(pwm);
  return true;
}

bool HardwareManager::initializeBuzzer(const GpioPinConfig &config) {
  buzzer = _hardwareFactory->createBuzzer(config);
  if (!buzzer) {
    Logger::error("Failed to initialize Buzzer on pin " +
                  std::to_string(config.pinNumber));
    return false;
  }
  return true;
}

void HardwareManager::onButtonEvent(int buttonId, bool pressed) {
  if (pressed) {
    handleButtonEvent(buttonId);
  }
}

void HardwareManager::handleButtonEvent(int buttonId) {
  auto buttonIdStr = findButtonIdByPin(buttonId);
  if (!buttonIdStr.empty()) {
    changeStateBasedOnButton(buttonId);
  }
}

void HardwareManager::changeStateBasedOnButton(int buttonId) {
  // Here you handle the state changes triggered by button presses
  // For example, using StateMachine
  StateMachine stateMachine;
  ButtonPressEvent pressEvent(buttonId);
  stateMachine.handleEvent(pressEvent);
}

std::string HardwareManager::findButtonIdByPin(int pin) {
  for (const auto &pair : _buttonIdToPinMap) {
    if (pair.second == pin) {
      return pair.first;
    }
  }
  return std::string(); // Return empty string if not found
}
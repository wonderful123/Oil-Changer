#include "HardwareInitializer.h"

HardwareInitializer::HardwareInitializer(
    std::shared_ptr<ConfigManager> configManager,
    std::shared_ptr<HardwareFactory> hardwareFactory,
    std::shared_ptr<ButtonController> buttonController,
    std::shared_ptr<BuzzerManager> buzzerManager,
    std::shared_ptr<IMediator> mediator)
    : _configManager(configManager),
      _hardwareFactory(hardwareFactory),
      _buttonController(buttonController),
      _buzzerManager(buzzerManager),
      _mediator(mediator) {}

Error HardwareInitializer::initialize(
    std::map<std::string, std::shared_ptr<HardwareComponent>>& components) {
  auto error = initializeComponents(components);
  if (error) return error;

  auto buzzerIt = components.find("Buzzer");
  if (buzzerIt != components.end()) {
    return initializeBuzzerManager(
        std::static_pointer_cast<IBuzzer>(buzzerIt->second));
  }

  return Error::HardwareConfigBuzzerInitError;  // Buzzer not found
}

Error HardwareInitializer::initializeComponents(
    std::map<std::string, std::shared_ptr<HardwareComponent>>& components) {
  Logger::info("[HardwareInitializer] Starting hardware initialization");

  // Get the hardware configuration object
  auto hardwareConfig = _configManager->getHardwareConfig();
  if (!hardwareConfig) {
    Logger::error(
        "[HardwareInitializer] Hardware configuration is not available");
    return Error::ConfigManagerHardwareConfigNotFound;  // Hardware config not
                                                        // found
  }

  // Loop through each hardware pin configuration and initialize
  for (const auto& pinConfig : hardwareConfig->getHardwarePinConfigs()) {
    std::shared_ptr<HardwareComponent> component;
    auto error = initializeComponent(pinConfig, component);
    if (error) {
      return error;  // Return specific error
    }
    components[pinConfig.id] = component;  // Store the initialized component
  }

  // Loop through display configurations if you have any (Optional)
  for (const auto& displayConfig : hardwareConfig->getDisplayConfigs()) {
    // Similar logic for initializing display components
  }

  return Error::OK;
}

Error HardwareInitializer::initializeComponent(
    const HardwarePinConfig& config,
    std::shared_ptr<HardwareComponent>& component) {
  component = _hardwareFactory->createComponent(config);
  if (!component) {
    Logger::error(
        "[HardwareInitializer] Failed to create/initialize component: " +
        config.id);
    return Error::HardwareComponentCreationFailure;
  }

  if (config.type == "Button") {
    if (auto button = std::static_pointer_cast<IButton>(component)) {
      _buttonController->registerButton(config.id, button);
    } else {
      Logger::error("[HardwareInitializer] Failed to cast to IButton: " +
                    config.id);
      return Error::ButtonRegistrationFailure;
    }
  }

  return Error::OK;  // Return OK if everything went well
}

Error HardwareInitializer::initializeBuzzerManager(
    std::shared_ptr<IBuzzer> buzzer) {
  if (!buzzer) {
    return Error::HardwareConfigBuzzerInitError;
  }

  auto interactionSettingsConfig =
      _configManager->getInteractionSettingsConfig();
  if (!interactionSettingsConfig) {
    return Error::ConfigManagerInteractionSettingsError;
  }

  auto settings = interactionSettingsConfig->getSettings();

  _buzzerManager = std::make_shared<BuzzerManager>(_mediator, buzzer, settings);

  return Error::OK;
}
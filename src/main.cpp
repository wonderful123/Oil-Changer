#include "ButtonController.h"
#include "BuzzerPlayer/BuzzerPlayer.h"
#include "ConfigManager.h"
#include "ESP32/ESP32Buzzer.h"
#include "ESP32/ESP32FileHandler.h"
#include "Error.h"
#include "FSM/States.h"
#include "HardwareConfig.h"
#include "HardwareManager.h"
#include "SystemController.h"
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <memory>
#include <tinyfsm.hpp>

ESP32FileHandler fileHandler;
std::shared_ptr<ConfigManager> configManager =
    std::make_shared<ConfigManager>(&fileHandler);
auto &hardwareFactory = HardwareFactory::getHardwareFactory();

std::shared_ptr<ButtonController> buttonController(new ButtonController());
std::shared_ptr<HardwareManager> hardwareManager(new HardwareManager(
    configManager, std::move(hardwareFactory), buttonController));
std::shared_ptr<SystemController>
    systemController(new SystemController(hardwareManager, buttonController));

std::unique_ptr<ESP32Buzzer> buzzer;
std::unique_ptr<BuzzerPlayer> player;

FSM_INITIAL_STATE(BaseState, IdleState);

void serialLogCallback(Logger::Level level, const std::string &message) {
  // Check if Serial is ready (important for some boards)
  if (!Serial) {
    return;
  }

  // Prefix each message with the log level
  switch (level) {
  case Logger::DEBUG:
    Serial.print("[DEBUG] ");
    break;
  case Logger::INFO:
    Serial.print("[INFO] ");
    break;
  case Logger::WARN:
    Serial.print("[WARN] ");
    break;
  case Logger::ERROR:
    Serial.print("[ERROR] ");
    break;
  default:
    Serial.print("[LOG] ");
  }

  // Print the message and a new line
  Serial.println(message.c_str());
}

void setup() {
  // Initialize Serial Communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial port to connect.
  }

  // Set the log callback to the function defined above
  Logger::setLogCallback(serialLogCallback);

  // Test logging
  Logger::info("Logger initialized and ready.");

  // Set up logging (assuming Logger setup code here)
  Logger::info("Loading hardware configuration...");
  Error configLoadError = configManager->loadConfig("HardwareConfig");
  if (configLoadError) {
    Logger::error("Failed to load hardware configuration: " +
                  Error::getFormattedMessage(configLoadError.code()));
    return;
  }

  // Initialize state machine
  IdleState::start(); // Start state machine in IdleState

  // Initialize system controller
  systemController->initialize();

  std::shared_ptr<HardwareConfig> hardwareConfig =
      configManager->getHardwareConfig();
  if (hardwareConfig) {
    const auto &gpioConfigs = hardwareConfig->getHardwarePinConfigs();
    for (const auto &gpioConfig : gpioConfigs) {
      if (gpioConfig.id == "Buzzer") {
        Logger::info("Initializing buzzer...");
        buzzer.reset(new ESP32Buzzer(gpioConfig));
        player.reset(new BuzzerPlayer(*buzzer));

        Logger::info("Playing super mario tune...");
        player->playTune(SUPER_MARIO_THEME);
        Logger::info("Playing starwars tune...");
        player->playTune(STARWARS_THEME);
        break; // Exit loop after initializing the buzzer
      }
    }
  } else {
    Logger::error("Hardware configuration not available.");
  }
}

void loop() {
  // Update the button states. This might involve polling or handling button
  // events.
  buttonController->checkButtonStates();

  // Update the system controller. This might involve processing events,
  // handling state changes, etc.
  systemController->update();
}

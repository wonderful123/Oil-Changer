#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>

#include <memory>
#include <tinyfsm.hpp>

#include "ButtonController.h"
#include "BuzzerPlayer/BuzzerPlayer.h"
#include "ConcreteMediator.h"
#include "ConfigManager.h"
#include "ESP32/ESP32Buzzer.h"
#include "ESP32/ESP32FileHandler.h"
#include "Error.h"
#include "FSM/States.h"
#include "HardwareConfig.h"
#include "HardwareFactory.h"
#include "HardwareManager.h"
#include "SystemController.h"

// Global objects
std::shared_ptr<ConfigManager> configManager;
std::shared_ptr<HardwareManager> hardwareManager;
std::shared_ptr<SystemController> systemController;

// Forward Declarations
void initializeLogger();
Error initializeHardware();
void initializeSystemController();
Error initializeBuzzerPlayer();
void serialLogCallback(Logger::Level level, const std::string &message);

void setup() {
  initializeLogger();

  auto mediator = std::make_shared<ConcreteMediator>();
  auto fileHandler = std::make_shared<ESP32FileHandler>();
  auto buttonController = std::make_shared<ButtonController>(mediator);
  auto hardwareFactory = HardwareFactory::getHardwareFactory();

  // Initialize global objects
  configManager = std::make_shared<ConfigManager>(mediator, fileHandler);
  hardwareManager = std::make_shared<HardwareManager>(
      mediator, configManager, hardwareFactory, buttonController);
  systemController = std::make_shared<SystemController>(
      mediator, buttonController, hardwareManager);
  Logger::info("[Main] System controller initialized.");

  auto oilChangeTracker =
      std::make_shared<OilChangeTracker>(mediator, configManager);

  // System initialization sequence
  if (initializeHardware()) {
    Logger::error("[Main] Hardware initialization failed.");
    return;  // Consider appropriate error handling or system halt
  }

  if (initializeBuzzerPlayer()) {
    Logger::warn("[Main] Buzzer player initialization failed.");
  }
}

void loop() { systemController->performPeriodicUpdate(); }

void initializeLogger() {
  Serial.begin(115200);
  while (!Serial)
    ;  // Wait for Serial port to connect

  Logger::setLogCallback(serialLogCallback);
  Logger::info(R"(
   ___  _ _  ___ _                           
  / _ \(_) |/ __| |_  __ _ _ _  __ _ ___ _ _ 
 | (_) | | | (__| ' \/ _` | ' \/ _` / -_) '_|
  \___/|_|_|\___|_||_\__,_|_||_\__, \___|_|  
                               |___/
    )");
  Logger::info("[Main] Logger initialized and ready.");
}

Error initializeHardware() {
  Logger::info("[Main] Loading hardware configuration...");
  Error configLoadError = configManager->loadConfig("HardwareConfig");
  if (configLoadError) {
    Logger::error("[Main] Failed to load hardware configuration: " +
                  Error::getFormattedMessage(configLoadError.code()));
    return configLoadError;
  }

  hardwareManager->initialize();
  
  return Error::OK;  // No error
}

Error initializeBuzzerPlayer() {
  auto hardwareConfig = configManager->getHardwareConfig();
  if (!hardwareConfig) {
    Logger::error("[Main] Hardware configuration not available.");
    return Error::HardwareConfigInitError;
  }

  for (const auto &gpioConfig : hardwareConfig->getHardwarePinConfigs()) {
    if (gpioConfig.id == "Buzzer") {
      Logger::info("[Main] Initializing buzzer...");
      auto buzzer = std::unique_ptr<ESP32Buzzer>(new ESP32Buzzer(gpioConfig));
      auto player = std::unique_ptr<BuzzerPlayer>(new BuzzerPlayer(*buzzer));

      Logger::info("[Main] Buzzer initialized.");

      Logger::info("[Main] Playing super mario tune...");
      // player->playTune(SUPER_MARIO_THEME);
      Logger::info("[Main] Playing starwars tune...");
      // player->playTune(STARWARS_THEME);
      Logger::info("[Main] Playing super mario with articulation tune ...");
      // player->playTune(SUPER_MARIO_ARTICULATION_THEME);
      Logger::info("[Main] Playing last ninja intro tune...");
      // player->playTune(LAST_NINJA_INTRO_THEME);
      Logger::info("[Main] Playing wonderboy theme tune...");
      // player->playTune(WONDERBOY_THEME);

      return Error::OK;  // Buzzer initialized successfully
    }
  }

  Logger::warn("[Main] Buzzer not found in hardware configuration.");
  return Error::HardwareConfigBuzzerInitError;  // Error code for missing buzzer
}

void serialLogCallback(Logger::Level level, const std::string &message) {
  if (!Serial) return;

  const char *levelStr[] = {"DEBUG", "INFO", "WARN", "ERROR", "LOG"};
  Serial.print('[');
  Serial.print(levelStr[static_cast<int>(level)]);
  Serial.print("] ");
  Serial.println(message.c_str());
}
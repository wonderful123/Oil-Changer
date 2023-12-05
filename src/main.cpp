#include "ButtonController.h"
#include "BuzzerPlayer/BuzzerPlayer.h"
#include "ConfigManager.h"
#include "DIContainer.h"
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

// Global objects
std::shared_ptr<SystemController> systemController;

// Forward Declarations
void initializeLogger();
Error initializeHardware();
void initializeSystemController();
Error initializeBuzzerPlayer();
void serialLogCallback(Logger::Level level, const std::string &message);

void setup() {
  initializeLogger();

  // Register dependencies in the DI Container
  DIContainer::resolve<ESP32FileHandler>();
  DIContainer::resolve<ConfigManager>();
  DIContainer::resolve<ButtonController>();
  DIContainer::resolve<HardwareFactory>();
  DIContainer::resolve<HardwareManager>();

  if (initializeHardware()) {
    Logger::error("[Main] Hardware initialization failed.");
    return; // Consider appropriate error handling or system halt
  }

  initializeSystemController();
  Logger::info("[Main] System controller initialized.");
  delay(1000);
  if (initializeBuzzerPlayer()) {
    Logger::warn("[Main] Buzzer player initialization failed.");
  }
}

void loop() { systemController->update(EventType::NOEVENT); }

void initializeLogger() {
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for Serial port to connect

  Logger::setLogCallback(serialLogCallback);
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
  return Error::OK; // No error
}

void initializeSystemController() {
  systemController = DIContainer::resolve<SystemController>();
  systemController->initialize();
  Logger::info("[Main] System controller initialized2.");
  delay(1000);
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

      return Error::OK; // Buzzer initialized successfully
    }
  }

  Logger::warn("[Main] Buzzer not found in hardware configuration.");
  return Error::HardwareConfigBuzzerInitError; // Error code for missing buzzer
}

void serialLogCallback(Logger::Level level, const std::string &message) {
  if (!Serial)
    return;

  const char *levelStr[] = {"DEBUG", "INFO", "WARN", "ERROR", "LOG"};
  Serial.print('[');
  Serial.print(levelStr[static_cast<int>(level)]);
  Serial.print("] ");
  Serial.println(message.c_str());
}
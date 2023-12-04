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

// Global objects
std::unique_ptr<ESP32FileHandler> fileHandler(new ESP32FileHandler());
std::shared_ptr<ConfigManager> configManager = std::make_shared<ConfigManager>(fileHandler.get());
std::shared_ptr<ButtonController> buttonController = std::make_shared<ButtonController>();
std::shared_ptr<SystemController> systemController;

// Forward Declarations
void initializeLogger();
Error initializeHardware();
void initializeSystemController();
Error initializeBuzzerPlayer();
void serialLogCallback(Logger::Level level, const std::string &message);

void setup() {
  initializeLogger();
  if (initializeHardware()) {
    Logger::error("Hardware initialization failed.");
    return; // Consider appropriate error handling or system halt
  }
  initializeSystemController();
  if (initializeBuzzerPlayer()) {
    Logger::warn("Buzzer player initialization failed.");
  }
}

void loop() { systemController->update(EventType::NOEVENT); }

void initializeLogger() {
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for Serial port to connect

  Logger::setLogCallback(serialLogCallback);
  Logger::info("Logger initialized and ready.");
}

Error initializeHardware() {
  Logger::info("Loading hardware configuration...");
  Error configLoadError = configManager->loadConfig("HardwareConfig");
  if (configLoadError) {
    Logger::error("Failed to load hardware configuration: " +
                  Error::getFormattedMessage(configLoadError.code()));
    return configLoadError;
  }
  return Error::OK; // No error
}

void initializeSystemController() {
  auto hardwareFactory = HardwareFactory::getHardwareFactory();
  std::shared_ptr<HardwareManager> hardwareManager =
      std::make_shared<HardwareManager>(
          configManager, std::move(hardwareFactory), buttonController);
  systemController =
      std::make_shared<SystemController>(hardwareManager, buttonController);
  systemController->initialize();
  IdleState::start(); // Start state machine in IdleState
  Logger::info("System controller initialized.");
}

Error initializeBuzzerPlayer() {
  auto hardwareConfig = configManager->getHardwareConfig();
  if (!hardwareConfig) {
    Logger::error("Hardware configuration not available.");
    return Error::HardwareConfigInitError;
  }

  for (const auto &gpioConfig : hardwareConfig->getHardwarePinConfigs()) {
    if (gpioConfig.id == "Buzzer") {
      Logger::info("Initializing buzzer...");
      auto buzzer = std::unique_ptr<ESP32Buzzer>(new ESP32Buzzer(gpioConfig));
      auto player = std::unique_ptr<BuzzerPlayer>(new BuzzerPlayer(*buzzer));

      Logger::info("Buzzer initialized.");

      Logger::info("Playing super mario tune...");
      // player->playTune(SUPER_MARIO_THEME);
      Logger::info("Playing starwars tune...");
      // player->playTune(STARWARS_THEME);
      Logger::info("Playing super mario with articulation tune ...");
      // player->playTune(SUPER_MARIO_ARTICULATION_THEME);
      Logger::info("Playing last ninja intro tune...");
      // player->playTune(LAST_NINJA_INTRO_THEME);
      Logger::info("Playing wonderboy theme tune...");
      // player->playTune(WONDERBOY_THEME);

      return Error::OK; // Buzzer initialized successfully
    }
  }

  Logger::warn("Buzzer not found in hardware configuration.");
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
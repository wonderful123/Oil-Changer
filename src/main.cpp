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
ESP32FileHandler fileHandler;
std::shared_ptr<ConfigManager> configManager = std::make_shared<ConfigManager>(&fileHandler);
std::shared_ptr<ButtonController> buttonController = std::make_shared<ButtonController>();
std::shared_ptr<SystemController> systemController;

// Forward Declarations
void initializeLogger();
void initializeHardware();
void initializeSystemController();
void initializeBuzzerPlayer();
void serialLogCallback(Logger::Level level, const std::string &message);

FSM_INITIAL_STATE(BaseState, IdleState);

void setup() {
  initializeLogger();
  initializeHardware();
  initializeSystemController();
  initializeBuzzerPlayer();
}

void loop() { systemController->update(); }

void initializeLogger() {
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for Serial port to connect

  Logger::setLogCallback(serialLogCallback);
  Logger::info("Logger initialized and ready.");
}

void initializeHardware() {
  Logger::info("Loading hardware configuration...");
  Error configLoadError = configManager->loadConfig("HardwareConfig");
  if (configLoadError) {
    Logger::error("Failed to load hardware configuration: " +
                  Error::getFormattedMessage(configLoadError.code()));
    return;
  }
}

void initializeSystemController() {
  auto &hardwareFactory = HardwareFactory::getHardwareFactory();
  std::shared_ptr<HardwareManager> hardwareManager =
      std::make_shared<HardwareManager>(
          configManager, std::move(hardwareFactory), buttonController);
  systemController =
      std::make_shared<SystemController>(hardwareManager, buttonController);
  systemController->initialize();
  IdleState::start(); // Start state machine in IdleState
}

void initializeBuzzerPlayer() {
  std::shared_ptr<HardwareConfig> hardwareConfig =
      configManager->getHardwareConfig();
  if (hardwareConfig) {
    const auto &gpioConfigs = hardwareConfig->getHardwarePinConfigs();
    for (const auto &gpioConfig : gpioConfigs) {
      if (gpioConfig.id == "Buzzer") {
        Logger::info("Initializing buzzer...");
        auto buzzer = std::unique_ptr<ESP32Buzzer>(new ESP32Buzzer(gpioConfig));
        auto player = std::unique_ptr<BuzzerPlayer>(new BuzzerPlayer(*buzzer));

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
        break; // Exit loop after initializing the buzzer
      }
    }
  } else {
    Logger::error("Hardware configuration not available.");
  }
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
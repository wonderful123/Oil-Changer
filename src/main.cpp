#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>

#include <memory>

#include "BuzzerPlayer/BuzzerPlayer.h"
#include "ConfigManager.h"
#include "ESP32/ESP32Buzzer.h"
#include "ESP32/ESP32FileHandler.h"
#include "Error.h"
#include "HardwareComponent.h"
#include "HardwareConfig.h"
#include "HardwareManager.h"
#include "IBuzzer.h"
#include "SystemController.h"
#include "SystemFactory.h"

// Global objects
std::shared_ptr<SystemController> systemController;
std::shared_ptr<HardwareManager> hardwareManager;
std::unique_ptr<BuzzerPlayer> player;
std::shared_ptr<IFileHandler> fileHandler;

// Forward Declarations
void initializeLogger();
Error initializeSystem();
Error initializeBuzzerPlayer();
void serialLogCallback(Logger::Level level, const std::string &message);

void setup() {
  initializeLogger();
  initializeSystem();
}

void loop() { systemController->performPeriodicUpdate(); }

void initializeLogger() {
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for Serial port to connect

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

Error initializeSystem() {
  fileHandler = std::shared_ptr<IFileHandler>(new ESP32FileHandler());
  SystemFactory &systemFactory = SystemFactory::getInstance();
  systemFactory.initializeSystem(fileHandler);
  systemController = systemFactory.getSystemController();
  hardwareManager = systemFactory.getHardwareManager();

  // if (!initializeBuzzerPlayer() != Error::OK) {
  //   Logger::warn("[Main] Buzzer player initialization failed");
  // }

  Logger::info("==== SYSTEM INITIALIZATION =====");
  Logger::info(" Status: SUCCESSFULLY COMPLETED");
  Logger::info("================================");

  return Error::OK;
}

Error initializeBuzzerPlayer() {
  auto buzzer = hardwareManager->getComponentById<IBuzzer>("Buzzer");
  if (buzzer) {
    player = std::unique_ptr<BuzzerPlayer>(new BuzzerPlayer(*buzzer));
    buzzer->adjustVolume(10);

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
    //  player->playTune(WONDERBOY_THEME);

    return Error::OK; // Buzzer initialized successfully
  }

  Logger::warn("[Main] Buzzer not found in HardwareManager");
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
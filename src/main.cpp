#include "ButtonController.h"
#include "BuzzerPlayer/BuzzerPlayer.h"
#include "ConfigManager.h"
#include "ESP32/Buzzer.h"
#include "ESP32/FileHandler.h"
#include "Error.h"
#include "HardwareConfig.h"
#include "HardwareManager.h"
#include "SystemController.h"
#include <Arduino.h>
#include <memory>

FileHandler fileHandler;
ConfigManager config(&fileHandler);
auto buttonController = std::make_shared<ButtonController>();
auto buttonController = std::make_shared<ButtonController>();

HardwareManager hardwareManager(configManager, std::move(hardwareFactory),
                                buttonController);
auto systemController =
    std::make_shared<SystemController>(hardwareManager, buttonController);

std::unique_ptr<Buzzer> buzzer;
std::unique_ptr<BuzzerPlayer> player;

void setup() {
  // Initialize Serial Communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial port to connect.
  }

  systemController.initialize();

  // Set up logging (assuming Logger setup code here)
  Logger::info("Loading hardware configuration...");
  Error configLoadError = config.loadConfig("HardwareConfig");
  if (configLoadError) {
    Logger::error("Failed to load hardware configuration: ");
    return;
  }

  std::shared_ptr<HardwareConfig> hardwareConfig = config.getHardwareConfig();
  if (hardwareConfig) {
    const auto &gpioConfigs = hardwareConfig->getGpioConfigs();
    for (const auto &gpioConfig : gpioConfigs) {
      if (gpioConfig.id == "Buzzer") {
        Logger::info("Initializing buzzer...");
        buzzer.reset(new Buzzer(gpioConfig));
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
  if (buzzer) {
    // buzzer->update();
  }
}
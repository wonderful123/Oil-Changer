#include <Arduino.h>
#include <BuzzerPlayer/BuzzerPlayer.h>
#include <Config.h>
#include <Hardware.h>

FileHandler fileHandler;
Config config(&fileHandler);
Buzzer *buzzer = nullptr;       // Initialize the pointer to nullptr
BuzzerPlayer *player = nullptr; // Initialize the player pointer to nullptr

void setup() {
  // Initialize Serial Communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial port to connect.
  }

  // Set the logger callback
  Logger::setLogCallback([](Logger::Level level, const char *message) {
    Serial.print("[");
    Serial.print(Logger::levelToString(level)); // Convert level to string
    Serial.print("] ");
    Serial.println(message);
  });

  // Set the Error class to use this logger
  Error::setLoggerCallback(Logger::log);

  Logger::info("Loading configuration...");
  config.load("/config.json");

  auto buzzerConfig = config.getConfigFor("BUZZER");
  if (!buzzerConfig.type.empty()) {
    Logger::info("Initializing buzzer...");
    buzzer = new Buzzer(buzzerConfig.gpio, buzzerConfig);

    Logger::info("Initializing BuzzerPlayer...");
    player = new BuzzerPlayer(*buzzer);

    Logger::info("Playing super mario tune...");
    player->playTune(SUPER_MARIO_THEME);
    Logger::info("Playing starwars tune...");
    player->playTune(STARWARS_THEME);
  } else {
    Logger::error("No buzzer configuration found.");
    Serial.println("No buzzer configuration found.");
  }
}

void loop() {
  if (buzzer) {
    buzzer->update();
  }
}


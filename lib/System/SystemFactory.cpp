// SystemFactory.cpp
#include "SystemFactory.h"

#include "HardwareFactory.h"
#include "Logger.h"

SystemFactory::SystemFactory(std::shared_ptr<IFileHandler> fileHandler)
    : fileHandler(fileHandler) {
  mediator = std::make_shared<ConcreteMediator>();  // Create a single mediator
                                                    // instance
  initializeSystem();
}

void SystemFactory::initializeSystem() {
  createConfigManager();

  createHardwareManager();
  hardwareManager->initialize();

  createSystemController();
  systemController->initializeSystemComponents();

  auto oilChangeTracker =
      std::make_shared<OilChangeTracker>(mediator, configManager);
  Logger::info("[SystemFactory] OilChangeTracker initialized.");
  Logger::info("[SystemFactory] System components initialized.");
}

std::shared_ptr<SystemController> SystemFactory::getSystemController() {
  return systemController;
}

std::shared_ptr<HardwareManager> SystemFactory::getHardwareManager() {
  return hardwareManager;
}

std::shared_ptr<ConfigManager> SystemFactory::getConfigManager() {
  return configManager;
}

void SystemFactory::createConfigManager() {
  configManager = std::make_shared<ConfigManager>(mediator, fileHandler);
}

void SystemFactory::createHardwareManager() {
  hardwareManager = std::make_shared<HardwareManager>(configManager);
}

void SystemFactory::createSystemController() {
  auto mediator = std::make_shared<ConcreteMediator>();
  systemController = std::make_shared<SystemController>(
      mediator, hardwareManager, configManager);
}

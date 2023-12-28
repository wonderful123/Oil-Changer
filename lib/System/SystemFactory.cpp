// SystemFactory.cpp
#include "SystemFactory.h"

#include "Logger.h"

SystemFactory::SystemFactory(std::shared_ptr<IFileHandler> fileHandler)
    : _fileHandler(fileHandler) {
  _mediator = std::make_shared<ConcreteMediator>();
}

void SystemFactory::initializeSystem() {
  Logger::info("[SystemFactory] Creating ConfigManager...");
  createConfigManager();

  Logger::info("[SystemFactory] Creating HardwareManager...");
  createHardwareManager();
  _hardwareManager->initialize();

  Logger::info("[SystemFactory] Creating SystemController...");
  createSystemController();
  _systemController->initializeSystemComponents();

  auto _oilChangeTracker =
      std::make_shared<OilChangeTracker>(_mediator, _configManager);
  Logger::info("[SystemFactory] OilChangeTracker initialized");
  Logger::info("[SystemFactory] All system components initialized");
}

std::shared_ptr<SystemController> SystemFactory::getSystemController() {
  return _systemController;
}

std::shared_ptr<HardwareManager> SystemFactory::getHardwareManager() {
  return _hardwareManager;
}

std::shared_ptr<ConfigManager> SystemFactory::getConfigManager() {
  return _configManager;
}

void SystemFactory::createConfigManager() {
  _configManager = std::make_shared<ConfigManager>(_mediator, _fileHandler);
}

void SystemFactory::createHardwareManager() {
  _hardwareManager = std::make_shared<HardwareManager>(_configManager);
}

void SystemFactory::createSystemController() {
  _systemController = std::make_shared<SystemController>(
      _mediator, _hardwareManager, _configManager);
}

// SystemFactory.cpp
#include "SystemFactory.h"

#include "Logger.h"

SystemFactory::SystemFactory(std::shared_ptr<IFileHandler> fileHandler)
    : _fileHandler(fileHandler) {
  _mediator = std::make_shared<ConcreteMediator>();
}

void SystemFactory::initializeSystem() {
  createConfigManager();
  createHardwareManager();
  createSystemController();
  createOilChangeTracker();

  Logger::info("[SystemFactory] All system components initialized");
}

std::shared_ptr<SystemController> SystemFactory::getSystemController() {
  return _systemController;
}

std::shared_ptr<HardwareManager> SystemFactory::getHardwareManager() {
  return _hardwareManager;
}

void SystemFactory::createConfigManager() {
  Logger::info("[SystemFactory] Creating ConfigManager...");
  ConfigManager::getInstance()->initialize(_mediator, _fileHandler);
}

void SystemFactory::createHardwareManager() {
  Logger::info("[SystemFactory] Creating HardwareManager...");
  _hardwareManager = std::make_shared<HardwareManager>();
  _hardwareManager->initialize();
}

void SystemFactory::createSystemController() {
  Logger::info("[SystemFactory] Creating SystemController...");
  _systemController =
      std::make_shared<SystemController>(_mediator, _hardwareManager);
  _systemController->initializeSystemComponents();
}

void SystemFactory::createOilChangeTracker() {
  Logger::info("[SystemFactory] Creating OilChangeTracker...");
  auto _oilChangeTracker = std::make_shared<OilChangeTracker>(_mediator);
}
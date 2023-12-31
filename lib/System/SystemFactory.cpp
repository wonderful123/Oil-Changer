// SystemFactory.cpp
#include "SystemFactory.h"
#include "ButtonController.h"
#include "ConfigManager.h"
#include "HardwareManager.h"
#include "Mediator/ConcreteMediator.h"
#include "OilChangeTracker.h"
#include "SystemController.h"

#include "Logger.h"

void SystemFactory::initializeSystem(
    std::shared_ptr<IFileHandler> fileHandler) {
  _fileHandler = fileHandler;
  _mediator = std::make_shared<ConcreteMediator>();
  createConfigManager();
  createHardwareManager();
  createSystemController();
  createOilChangeTracker();

  Logger::info("[SystemFactory] All system components initialized");
}

std::shared_ptr<IMediator> SystemFactory::getMediator() { return _mediator; }

std::shared_ptr<SystemController> SystemFactory::getSystemController() {
  return _systemController;
}

std::shared_ptr<HardwareManager> SystemFactory::getHardwareManager() {
  return _hardwareManager;
}

std::shared_ptr<ButtonController> SystemFactory::getButtonController() {
  if (!_systemController) {
    // Handle the case where _systemController isn't set up yet
    Logger::error("[SystemFactory] SystemController is not initialized.");
    return nullptr;
  }
  return _systemController->getButtonController();
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
  auto _oilChangeTracker = OilChangeTracker::getInstance(_mediator);
}

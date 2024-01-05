// SystemFactory.cpp
#include "SystemFactory.h"
#include "ButtonController.h"
#include "ConfigManager.h"
#include "EventManager/EventManager.h"
#include "FSM/StateMachine.h"
#include "FSM/States.h"
#include "HardwareManager.h"
#include "Logger.h"
#include "OilChangeTracker.h"
#include "SystemController.h"

FSM_INITIAL_STATE(StateMachine, Initializing);

void SystemFactory::initializeSystem(
    std::shared_ptr<IFileHandler> fileHandler) {
  _fileHandler = fileHandler;
  _eventManager = std::make_shared<EventManager>();
  createConfigManager();
  createHardwareManager();
  createSystemController();
  createOilChangeTracker();
  createStateMachine(_eventManager, _buzzerManager);
  
  Logger::info("[SystemFactory] All system components initialized");
  sendFSMEvent(InitializationCompleteEvent());
}

void SystemFactory::createEventManager() {
  Logger::info("[SystemFactory] Creating EventManager...");
  _eventManager = std::make_shared<EventManager>();
}

void SystemFactory::createConfigManager() {
  Logger::info("[SystemFactory] Creating ConfigManager...");
  ConfigManager::getInstance()->initialize(_fileHandler);
}

void SystemFactory::createHardwareManager() {
  Logger::info("[SystemFactory] Creating HardwareManager...");
  _hardwareManager = std::make_shared<HardwareManager>();
  _hardwareManager->initialize();
}

void SystemFactory::createSystemController() {
  Logger::info("[SystemFactory] Creating SystemController...");
  _systemController = std::make_shared<SystemController>(_hardwareManager);
  _systemController->initializeSystemComponents();
  _buzzerManager = _systemController->getBuzzerManager();
}

void SystemFactory::createOilChangeTracker() {
  Logger::info("[SystemFactory] Creating OilChangeTracker...");
  _oilChangeTracker = std::make_shared<OilChangeTracker>(_eventManager);
}

void SystemFactory::createStateMachine(std::shared_ptr <EventManager> eventManager, std::shared_ptr<BuzzerManager> buzzerManager) {
  StateMachine::setSharedResources(eventManager, buzzerManager);
  StateMachine::start();
  Logger::info("[SystemFactory] Created StateMachine");
}

std::shared_ptr<EventManager> SystemFactory::getEventManager() {
  return _eventManager;
}

std::shared_ptr<SystemController> SystemFactory::getSystemController() {
  return _systemController;
}

std::shared_ptr<BuzzerManager> SystemFactory::getBuzzerManager() {
  return _buzzerManager;
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

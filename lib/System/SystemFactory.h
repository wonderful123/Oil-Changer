// SystemFactory.h
#pragma once

#include <memory>

#include "ConcreteMediator.h"
#include "ConfigManager.h"
#include "HardwareManager.h"
#include "IFileHandler.h"
#include "OilChangeTracker.h"
#include "SystemController.h"

class SystemFactory {
 public:
  explicit SystemFactory(std::shared_ptr<IFileHandler> fileHandler);
  void initializeSystem();
  std::shared_ptr<SystemController> getSystemController();
  std::shared_ptr<HardwareManager> getHardwareManager();
  std::shared_ptr<ConfigManager> getConfigManager();

 private:
  std::shared_ptr<ConfigManager> configManager;
  std::shared_ptr<HardwareManager> hardwareManager;
  std::shared_ptr<SystemController> systemController;
  std::shared_ptr<IFileHandler> fileHandler;
  std::shared_ptr<IMediator> mediator;
  std::shared_ptr<OilChangeTracker> oilChangeTracker;

  void createConfigManager();
  void createHardwareManager();
  void createSystemController();
};

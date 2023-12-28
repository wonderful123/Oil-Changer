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
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<SystemController> _systemController;
  std::shared_ptr<IFileHandler> _fileHandler;
  std::shared_ptr<IMediator> _mediator;
  std::shared_ptr<OilChangeTracker> _oilChangeTracker;

  void createConfigManager();
  void createHardwareManager();
  void createSystemController();
};

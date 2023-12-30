// SystemFactory.h
#pragma once

#include <memory>

class IMediator;
class ConfigManager;
class IFileHandler;
class HardwareManager;
class OilChangeTracker;
class SystemController;
class ButtonController;

class SystemFactory {
public:
  static SystemFactory &getInstance() {
    static SystemFactory instance;
    return instance;
  }

  void initializeSystem(std::shared_ptr<IFileHandler> fileHandler);

  std::shared_ptr<IMediator> getMediator();
  std::shared_ptr<SystemController> getSystemController();
  std::shared_ptr<HardwareManager> getHardwareManager();
  std::shared_ptr<ButtonController> getButtonController();

private:
  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<SystemController> _systemController;
  std::shared_ptr<SystemController> _buttonController;
  std::shared_ptr<IFileHandler> _fileHandler;
  std::shared_ptr<IMediator> _mediator;
  std::shared_ptr<OilChangeTracker> _oilChangeTracker;

  void createConfigManager();
  void createHardwareManager();
  void createSystemController();
  void createOilChangeTracker();

  // Private Constructor
  SystemFactory() {}

  // Delete copy constructor and assignment operator to prevent copies
  SystemFactory(SystemFactory const &) = delete;            // Copy construct
  SystemFactory &operator=(SystemFactory const &) = delete; // Copy assign
};

// SystemFactory.h
#pragma once

#include <memory>

class EventManager;
class ConfigManager;
class IFileHandler;
class HardwareManager;
class OilChangeTracker;
class SystemController;
class ButtonController;
class BuzzerManager;

class SystemFactory {
public:
  static SystemFactory &getInstance() {
    static SystemFactory instance;
    return instance;
  }

  void initializeSystem(std::shared_ptr<IFileHandler> fileHandler);

  std::shared_ptr<EventManager> getEventManager();
  std::shared_ptr<SystemController> getSystemController();
  std::shared_ptr<HardwareManager> getHardwareManager();
  std::shared_ptr<ButtonController> getButtonController();
  std::shared_ptr<BuzzerManager> getBuzzerManager();

private:
  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<SystemController> _systemController;
  std::shared_ptr<BuzzerManager> _buzzerManager;
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<IFileHandler> _fileHandler;
  std::shared_ptr<EventManager> _eventManager;
  std::shared_ptr<OilChangeTracker> _oilChangeTracker;

  void createEventManager();
  void createStateMachine(std::shared_ptr<EventManager> eventManager,
                          std::shared_ptr<BuzzerManager> buzzerManager);
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

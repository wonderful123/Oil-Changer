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
class DisplayManager;
class InteractionSettings;
class MotorController;
class SensorManager;

class SystemFactory {
public:
  static SystemFactory &getInstance();

  void initializeSystem(std::shared_ptr<IFileHandler> fileHandler);

  std::shared_ptr<EventManager> getEventManager() const;
  std::shared_ptr<SystemController> getSystemController() const;
  std::shared_ptr<HardwareManager> getHardwareManager() const;
  std::shared_ptr<ButtonController> getButtonController() const;
  std::shared_ptr<BuzzerManager> getBuzzerManager() const;
  std::shared_ptr<ConfigManager> getConfigManager() const;
  std::shared_ptr<DisplayManager> getDisplayManager() const;
  std::shared_ptr<SensorManager> getSensorManager() const;

private:
  SystemFactory() {}

  void createEventManager();
  void createStateMachine();
  void createConfigManager();
  void createHardwareManager();
  void createSystemController();
  void createOilChangeTracker();
  void createDisplayManager();
  void createButtonController();
  void createBuzzerManager();
  void createMotorController();
  void createSensorManager();

  bool loadInteractionSettings();

  std::shared_ptr<HardwareManager> _hardwareManager;
  std::shared_ptr<SystemController> _systemController;
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<IFileHandler> _fileHandler;
  std::shared_ptr<EventManager> _eventManager;
  std::shared_ptr<OilChangeTracker> _oilChangeTracker;
  std::shared_ptr<DisplayManager> _displayManager;
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<BuzzerManager> _buzzerManager;
  std::shared_ptr<MotorController> _motorController;
  std::shared_ptr<InteractionSettings> _interactionSettings;
  std::shared_ptr<SensorManager> _sensorManager;

  // Non-copyable
  SystemFactory(const SystemFactory &) = delete;
  SystemFactory &operator=(const SystemFactory &) = delete;
};

// SystemController.h
#pragma once

#include <memory>

class ButtonController;
class MotorController;
class SensorManager;
class DisplayManager;

class SystemController {
public:
  SystemController(std::shared_ptr<ButtonController> buttonController,
                   std::shared_ptr<MotorController> motorController,
                   std::shared_ptr<SensorManager> sensorManager,
                   std::shared_ptr<DisplayManager> displayManager);

  void performPeriodicUpdate();

private:
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<MotorController> _motorController;
  std::shared_ptr<SensorManager> _sensorManager;
  std::shared_ptr<DisplayManager> _displayManager;
};

// SystemController.cpp

#include "SystemController.h"

#include <memory>

// Project-specific headers
#include "ButtonController.h"
#include "DisplayManager.h"
#include "Logger.h"
#include "MotorController.h"
#include "SensorManager.h"

SystemController::SystemController(
    std::shared_ptr<ButtonController> buttonController,
    std::shared_ptr<MotorController> motorController,
    std::shared_ptr<SensorManager> sensorManager,
    std::shared_ptr<DisplayManager> displayManager)
    : _buttonController(buttonController), _motorController(motorController),
      _sensorManager(sensorManager), _displayManager(displayManager) {}

void SystemController::performPeriodicUpdate() {
  if (_buttonController)
    _buttonController->processButtonStates();
  if (_motorController)
    _motorController->update(); // For motor speed ramping
  if (_sensorManager)
    _sensorManager->update();
  if (_displayManager)
    _displayManager->update();
}

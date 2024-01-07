// MotorController.cpp
#include "MotorController.h"
#include "Error.h"
#include "EventManager/EventManager.h"

MotorController::MotorController(std::shared_ptr<EventManager> eventManager)
    : _eventManager(eventManager),
      _rampState(0.0f, 0.0f, std::chrono::steady_clock::now(),
                 std::chrono::milliseconds(0), false) {}

MotorController::~MotorController() {
  // Unsubscribe from Motor events
  _eventManager->unsubscribe(shared_from_this(), Event::Motor);
}

void MotorController::initialize(std::shared_ptr<IDAC> dac,
                                 std::shared_ptr<IDigitalIO> fill,
                                 std::shared_ptr<IDigitalIO> extract) {
  // Subscribe to Motor events
  _eventManager->subscribe(shared_from_this(), Event::Motor);

  registerDacComponent(dac);
  registerDigitalIO(fill, extract);
}

void MotorController::onNotify(Event type, Parameter parameter, float value) {
  if (type == Event::Motor) {
    switch (parameter) {
    case Parameter::MotorFill:
      activateFill(true);
      break;
    case Parameter::MotorExtract:
      activateExtract(true);
      break;
    case Parameter::MotorStop:
      stopMotor();
      break;
    default:
      break;
    }
  }
}

void MotorController::registerDacComponent(std::shared_ptr<IDAC> dac) {
  _dacComponent = std::move(dac);
}

void MotorController::registerDigitalIO(std::shared_ptr<IDigitalIO> fill,
                                        std::shared_ptr<IDigitalIO> extract) {
  _fill = std::move(fill);
  _extract = std::move(extract);
}

void MotorController::activateFill(bool enabled) {
  if (enabled && _extract) {
    _extract->write(IDigitalIO::DIGITAL_LOW); // Ensure extract is off
  }
  if (_fill) {
    _fill->write(enabled ? IDigitalIO::DIGITAL_HIGH : IDigitalIO::DIGITAL_LOW);
  }
}

void MotorController::activateExtract(bool enabled) {
  if (enabled && _fill) {
    _fill->write(IDigitalIO::DIGITAL_LOW); // Ensure fill is off
  }
  if (_extract) {
    _extract->write(enabled ? IDigitalIO::DIGITAL_HIGH
                            : IDigitalIO::DIGITAL_LOW);
  }
}

void MotorController::stopMotor() {
  if (_fill) {
    _fill->write(IDigitalIO::DIGITAL_LOW);
  }
  if (_extract) {
    _extract->write(IDigitalIO::DIGITAL_LOW);
  }
  if (_dacComponent) {
    _dacComponent->setValue(0); // Stop the motor by setting DAC value to 0
  }
}

void MotorController::setMaxSpeed(float maxSpeed) { _maxSpeed = maxSpeed; }

// Get the maximum speed of the motor
float MotorController::getMaxSpeed() const { return _maxSpeed; }

// Set the motor speed as a fraction (0 to 1) of the maximum speed
void MotorController::setMotorSpeed(float fraction) {
  if (fraction < 0.0f || fraction > 1.0f) {
    // Handle invalid input
    return;
  }

  _currentFraction = fraction;
  if (_dacComponent) {
    _dacComponent->setValue(static_cast<int>(_maxSpeed * fraction));
  }
}

void MotorController::startRamp(float targetFraction,
                                std::chrono::milliseconds duration) {
  _rampState = {_currentFraction, targetFraction,
                std::chrono::steady_clock::now(), duration, true};
}

void MotorController::update() {
  if (!_rampState.isRamping)
    return;

  auto elapsed = std::chrono::steady_clock::now() - _rampState.startTime;
  if (elapsed >= _rampState.duration) {
    setMotorSpeed(_rampState.targetFraction);
    _rampState.isRamping = false;
  } else {
    float elapsedFraction =
        static_cast<float>(elapsed.count()) / _rampState.duration.count();
    setMotorSpeed(_rampState.startFraction +
                  (_rampState.targetFraction - _rampState.startFraction) *
                      elapsedFraction);
  }
}

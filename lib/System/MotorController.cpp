// MotorController.cpp
#include "MotorController.h"
#include "Error.h"
#include "EventManager/EventManager.h"

MotorController::MotorController(std::shared_ptr<EventManager> eventManager,
                                 MotorSettings &motorSettings)
    : _eventManager(eventManager),
      _rampState(0.0f, 0.0f, std::chrono::steady_clock::now(),
                 std::chrono::milliseconds(0), false) {
  updateMotorSettings(motorSettings);
}

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
      activateFill();
      break;
    case Parameter::MotorExtract:
      activateExtract();
      break;
    case Parameter::MotorStop:
      stopMotor();
      break;
    case Parameter::MotorHalt:
      haltMotor();
      break;
    default:
      break;
    }
  }
}

void MotorController::updateMotorSettings(MotorSettings &motorSettings) {
  _settings = motorSettings;
}

void MotorController::registerDacComponent(std::shared_ptr<IDAC> dac) {
  _dacComponent = std::move(dac);
}

void MotorController::registerDigitalIO(std::shared_ptr<IDigitalIO> fill,
                                        std::shared_ptr<IDigitalIO> extract) {
  _fill = std::move(fill);
  _extract = std::move(extract);
}

void MotorController::activateFill() {
  if (_extract) {
    _extract->write(IDigitalIO::DIGITAL_LOW); // Ensure extract is off
  }

  if (_fill) {
    if (_settings.rampingEnabled) {
      startRamp(_settings.maxSpeed,
                std::chrono::milliseconds(
                    _settings.rampingOnDurationMs)); // Ramp up to max speed
    } else {
      _fill->write(IDigitalIO::DIGITAL_HIGH);
    }
  }
}

void MotorController::activateFill() {
  if (_fill) {
    _fill->write(IDigitalIO::DIGITAL_LOW); // Ensure extract is off
  }

  if (_extract) {
    if (_settings.rampingEnabled) {
      startRamp(_settings.maxSpeed,
                std::chrono::milliseconds(
                    _settings.rampingOnDurationMs)); // Ramp up to max speed
    } else {
      _extract->write(IDigitalIO::DIGITAL_HIGH);
    }
  }
}

void MotorController::stopMotor() {
  if (_settings.rampingEnabled) {
    startRamp(0.0, std::chrono::milliseconds(
                       _settings.rampingOffDurationMs)); // Ramp down to 0
  } else {
    haltMotor();
  }
}

// Stop right away and ignore any ramping if enabled
void MotorController::haltMotor() {
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

void MotorController::setMaxSpeed(float maxSpeed) {
  _settings.maxSpeed = maxSpeed;
}

// Get the maximum speed of the motor
float MotorController::getMaxSpeed() const { return _settings.maxSpeed; }

// Set the motor speed as a fraction (0 to 1) of the maximum speed
void MotorController::setMotorSpeed(float fraction) {
  // Clamp speed between 0 and 1
  if (fraction < 0) {
    fraction = 0;
  } else if (fraction > 1) {
    fraction = 1;
  }
  _currentFraction = fraction;

  // Convert as fraction of max speed
  float convertedValue = _settings.maxSpeed * fraction;
  if (_dacComponent) {
    _dacComponent->setValue(convertedValue);
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

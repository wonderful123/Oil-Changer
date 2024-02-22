#pragma once

#include "EventManager/EventManager.h"
#include "IDAC.h"
#include "IDigitalIO.h"
#include "MotorSettings.h"
#include <chrono>
#include <memory>

class MotorController : public IEventListener,
                        public std::enable_shared_from_this<MotorController> {
public:
  MotorController(std::shared_ptr<EventManager> eventManager,
                  MotorSettings &motorSettings);
  virtual ~MotorController() override;

  void initialize(std::shared_ptr<IDAC> motorSpeedDac,
                  std::shared_ptr<IDigitalIO> fillDirectionSwitch,
                  std::shared_ptr<IDigitalIO> extractDirectionSwitch);

  // Implement onNotify from IEventListener
  virtual void onNotify(Event type, Parameter parameter) override;

  void updateMotorSettings(MotorSettings &motorSettings);

  // Method to register DAC component for motor speed control
  void registerDacComponent(std::shared_ptr<IDAC> motorSpeedDac);
  // Method to register digital IO components for direction control
  void registerDigitalIO(std::shared_ptr<IDigitalIO> fillDirectionSwitch,
                         std::shared_ptr<IDigitalIO> extractDirectionSwitch);

  // Stops motor straight away and ignores any ramping
  void haltMotor();

  // Set the maximum speed for the motor
  void setMaxSpeed(float maxSpeed);

  // Get the maximum speed of the motor
  float getMaxSpeed() const;

  // Set the motor speed as a fraction (0 to 1) of the maximum speed
  void setMotorSpeed(float fraction);

  // Method to initiate ramping
  void startRamp(float targetFraction, std::chrono::milliseconds duration);

  // Call this regularly to update the motor speed non-blockingly
  void update();

private:
  std::shared_ptr<EventManager> _eventManager;
  std::shared_ptr<IDAC> _motorSpeedDac; // DAC component for motor speed control
  std::shared_ptr<IDigitalIO>
      _fillDirectionSwitch; // Digital IO for fill direction control
  std::shared_ptr<IDigitalIO>
      _extractDirectionSwitch; // Digital IO for extract direction control

  MotorSettings _settings;

  float _currentFraction; // Current speed as a fraction of the maximum speed

  struct RampState {
    float startFraction;
    float targetFraction;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::milliseconds duration;
    bool isRamping;

    RampState() = default;

    RampState(float startFraction, float targetFraction,
              std::chrono::steady_clock::time_point startTime,
              std::chrono::milliseconds duration, bool isRamping)
        : startFraction(startFraction), targetFraction(targetFraction),
          startTime(startTime), duration(duration), isRamping(isRamping) {}
  } _rampState;

  // Helper methods to handle motor control
  void activateFillDirection();
  void activateExtractDirection();
  void stopMotor();
};

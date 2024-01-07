// MotorController.h
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

  void initialize(std::shared_ptr<IDAC> dac, std::shared_ptr<IDigitalIO> fill,
                  std::shared_ptr<IDigitalIO> extract);

  // Implement onNotify from IEventListener
  virtual void onNotify(Event type, Parameter parameter, float value) override;

  void updateMotorSettings(MotorSettings &motorSettings);

  // Method to register DAC component
  void registerDacComponent(std::shared_ptr<IDAC> dac);
  // Method to register digital IO components
  void registerDigitalIO(std::shared_ptr<IDigitalIO> fill,
                         std::shared_ptr<IDigitalIO> extract);

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
  std::shared_ptr<IDAC> _dacComponent;  // DAC component for motor speed control
  std::shared_ptr<IDigitalIO> _fill;    // Digital IO for fill component
  std::shared_ptr<IDigitalIO> _extract; // Digital IO for extract component

  MotorSettings _settings;

  float _currentFraction; // Current speed as a fraction of the maximum speed

  struct RampState {
    float startFraction;
    float targetFraction;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::milliseconds duration;
    bool isRamping;

    RampState() = default;

    // Optionally, provide a constructor for convenience
    RampState(float startFraction, float targetFraction,
              std::chrono::steady_clock::time_point startTime,
              std::chrono::milliseconds duration, bool isRamping)
        : startFraction(startFraction), targetFraction(targetFraction),
          startTime(startTime), duration(duration), isRamping(isRamping) {}
  } _rampState;

  // Helper methods to handle motor control
  void activateFill();
  void activateExtract();
  void stopMotor();
};
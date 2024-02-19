// SensorManager.h
#pragma once

#include "EventManager/EventManager.h"
#include "EventManager/IEventListener.h"
#include <memory>

class IADC;
class IDigitalIO;
class IFlowMeter;
class OilChangeTracker;

struct SensorManagerComponents {
  std::shared_ptr<OilChangeTracker> oilChangeTracker;
  std::shared_ptr<IADC> voltageSensor;
  std::shared_ptr<IADC> oilTemperatureSensor;
  std::shared_ptr<IDigitalIO> fillPressureSwitch;
  std::shared_ptr<IDigitalIO> extractPressureSwitch;
  std::shared_ptr<IFlowMeter> fillFlowMeter;
  std::shared_ptr<IFlowMeter> extractFlowMeter;
};

class SensorManager : public IEventListener,
                      public std::enable_shared_from_this<OilChangeTracker> {
public:
  SensorManager(std::shared_ptr<EventManager> eventManager);
  ~SensorManager();

  void initialize(SensorManagerComponents components);

  // Update sensor readings (call in main loop)
  void update();

  void resetFlowMetersVolume();

private:
  void onNotify(Event event, Parameter parameter, float value) override;

  // Private methods to read individual sensors
  void readVoltageSensor();
  void readOilTemperatureSensor();
  void readFillPressureSwitch();
  void readExtractPressureSwitch();
  void readFillFlowMeter();
  void readExtractFlowMeter();

  std::shared_ptr<OilChangeTracker> _oilChangeTracker;
  std::shared_ptr<IADC> _voltageSensor;
  std::shared_ptr<IADC> _oilTemperatureSensor;
  std::shared_ptr<IDigitalIO> _fillPressureSwitch;
  std::shared_ptr<IDigitalIO> _extractPressureSwitch;
  std::shared_ptr<IFlowMeter> _fillFlowMeter;
  std::shared_ptr<IFlowMeter> _extractFlowMeter;

  // Variables to store LPS switch states
  int _fillPressureSwitchState;
  int _extractPressureSwitchState;
};
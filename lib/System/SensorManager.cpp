// SensorManager.cpp
#include "SensorManager.h"
#include "FSM/StateMachine.h"
#include "IADC.h"
#include "IDigitalIO.h"
#include "IFlowMeter.h"
#include "OilChangeTracker.h"

SensorManager::SensorManager() {}
SensorManager::~SensorManager() {}

void SensorManager::initialize(SensorManagerComponents components) {
  _oilChangeTracker = components.oilChangeTracker;
  _oilTemperatureSensor = components.oilTemperatureSensor;
  _voltageSensor = components.voltageSensor;
  _fillPressureSwitch = components.fillPressureSwitch;
  _extractPressureSwitch = components.extractPressureSwitch;
  _fillFlowMeter = components.fillFlowMeter;
  _extractFlowMeter = components.extractFlowMeter;
}

void SensorManager::update() {
  readFillPressureSwitch();
  readExtractPressureSwitch();
  readFillFlowMeter();
  readExtractFlowMeter();
  readOilTemperatureSensor();
  readVoltageSensor();
}

void SensorManager::readVoltageSensor() {
  // TODO: implement with the voltage divider values
  int voltageSensorADCValue = _voltageSensor->read();
  int adcResolution = _voltageSensor->resolution();
  double voltage = voltageSensorADCValue * (3.3 / adcResolution);
  _oilChangeTracker->setVoltage(voltage);
}

void SensorManager::readOilTemperatureSensor() {
  // TODO: implement with the voltage divider values
  int temperatureSensorADCValue = _oilTemperatureSensor->read();
  double temperature = 100.0;
  _oilChangeTracker->setOilTemperature(temperature);
}

void SensorManager::readFillPressureSwitch() {
  int newSwitchState = _fillPressureSwitch->read();
  if (newSwitchState != _fillPressureSwitchState) {
    _fillPressureSwitchState = newSwitchState;
    _oilChangeTracker->setFillLPSState(newSwitchState);
    sendFSMEvent(PressureSwitchEvent(PressureSwitch::Fill, newSwitchState));
  }
}

void SensorManager::readExtractPressureSwitch() {
  int newSwitchState = _extractPressureSwitch->read();
  if (newSwitchState != _extractPressureSwitchState) {
    _extractPressureSwitchState = newSwitchState;
    sendFSMEvent(PressureSwitchEvent(PressureSwitch::Extract, newSwitchState));
  }
}

void SensorManager::readFillFlowMeter() {
  _fillFlowMeter->update();
  _oilChangeTracker->setFillFlowRate(_fillFlowMeter->getFlowRate());
  _oilChangeTracker->incrementAmountFilled(_fillFlowMeter->getTotalVolume());
}

void SensorManager::readExtractFlowMeter() {
  _extractFlowMeter->update();
  _oilChangeTracker->setFillFlowRate(_extractFlowMeter->getFlowRate());
  _oilChangeTracker->incrementAmountFilled(_extractFlowMeter->getTotalVolume());
}

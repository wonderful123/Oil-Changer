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
  int voltageSensorADCValue = _voltageSensor->read();
  int adcResolution =
      _voltageSensor->resolution(); // Assuming it's 4096 for 12-bit ADC
  double Vout = voltageSensorADCValue * (3.3 / adcResolution);

  // Values for R1 and R2 in the voltage divider
  double R1 = 68000; // 68k ohm
  double R2 = 10000; // 10k ohm

  // Calculate Vin (actual voltage before the divider)
  double Vin = Vout * ((R1 + R2) / R2);

  _oilChangeTracker->setVoltage(Vin);
}

void SensorManager::readOilTemperatureSensor() {
  int temperatureSensorADCValue = _oilTemperatureSensor->read();
  double Vin = 3.3;                                // Supply voltage
  double R1 = 56000; // Resistance of known resistor in voltage divider
  double R0 = 50000; // Resistance of thermistor at reference temperature (25°C)
  double T0 = 298.15; // Reference temperature in Kelvin (25°C)
  double B = 3950;    // Beta coefficient of the thermistor

  // Calculate Vout from ADC value
  double Vout = (double)temperatureSensorADCValue *
                (Vin / _oilTemperatureSensor->resolution());

  // Calculate thermistor resistance
  double R2 = (Vout * R1) / (Vin - Vout);

  // Calculate temperature in Kelvin
  double temperatureK = 1 / (1 / T0 + (1 / B) * log(R2 / R0));

  // Convert temperature to Celsius
  double temperatureC = temperatureK - 273.15;

  _oilChangeTracker->setOilTemperature(temperatureC);
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

// OilChangeTracker.cpp
#include "OilChangeTracker.h"
#include "EventManager/EventManager.h"
#include "FSM/StateMachine.h"
#include "Logger.h"
#include "SystemFactory.h"

OilChangeTracker::OilChangeTracker(std::shared_ptr<EventManager> eventManager)
    : _fillCapacity(4.0f), _amountFilled(0), _amountExtracted(0),
      _fillFlowRate(0), _extractFlowRate(0), _voltage(0),
      _eventManager(eventManager) {}

void OilChangeTracker::initialize() {
  _eventManager->subscribe(shared_from_this(), Event::OilChangeTracker);
}

void OilChangeTracker::onNotify(Event event, Parameter parameter, float value) {
  if (event == Event::OilChangeTracker) {
    // 'id' is used to specify what to update (e.g., fill capacity)
    if (parameter == Parameter::FillCapacity) {
      // Check the value to determine increment or decrement
      if (value > 0) {
        incrementFillCapacity(value);
      } else if (value < 0) {
        decrementFillCapacity(value);
      }
      Logger::info("[OilChangeTracker] - Fill Capacity: " +
                   std::to_string(_fillCapacity));
    } else if (parameter == Parameter::Reset) {
      reset();
    }
  }
}

OilChangeTracker::TrackerData OilChangeTracker::getCurrentData() const {
  TrackerData data;
  data.fillCapacity = _fillCapacity;
  data.amountFilled = _amountFilled;
  data.amountExtracted = _amountExtracted;
  data.fillFlowRate = _fillFlowRate;
  data.extractFlowRate = _extractFlowRate;
  data.voltage = _voltage;
  data.oilTemperature = _oilTemperature;
  data.fillLPSState = _fillLPSState;
  data.extractLPSState = _extractLPSState;
  return data;
}

void OilChangeTracker::reset() {
  _fillCapacity = 4.0f;
  _amountFilled = 0;
  _amountExtracted = 0;
}

// Define the setter methods
void OilChangeTracker::incrementAmountFilled(double amount) {
  _amountFilled += amount;
  if (_amountFilled >= _fillCapacity) {
    sendFSMEvent(OilFillTargetReachedEvent());
  }
}

void OilChangeTracker::incrementAmountExtracted(double amount) {
  _amountExtracted += amount;
}

void OilChangeTracker::resetAmountFilled() { _amountFilled = 0; }

void OilChangeTracker::resetAmountExtracted() { _amountExtracted = 0; }

void OilChangeTracker::setFillFlowRate(double rate) { _fillFlowRate = rate; }

void OilChangeTracker::setExtractFlowRate(double rate) {
  _extractFlowRate = rate;
}

void OilChangeTracker::setVoltage(double volt) { _voltage = volt; }

void OilChangeTracker::setOilTemperature(double temperature) {
  _oilTemperature = temperature;
}

void OilChangeTracker::setFillLPSState(int state) {
  if (_fillLPSState != state) {
    sendFSMEvent(PressureSwitchEvent(PressureSwitch::Fill, state));
    _fillLPSState = state;
  }
}

void OilChangeTracker::setExtractLPSState(int state) {
  if (_extractLPSState != state) {
    sendFSMEvent(PressureSwitchEvent(PressureSwitch::Extract, state));
    _extractLPSState = state;
  }
}

void OilChangeTracker::incrementFillCapacity(double amount) {
  _fillCapacity += amount;
}

void OilChangeTracker::decrementFillCapacity(double amount) {
  _fillCapacity -= amount;
}

// Define the getter methods
double OilChangeTracker::fillCapacity() const { return _fillCapacity; }

double OilChangeTracker::amountFilled() const { return _amountFilled; }

double OilChangeTracker::amountExtracted() const { return _amountExtracted; }

double OilChangeTracker::fillFlowRate() const { return _fillFlowRate; }

double OilChangeTracker::extractFlowRate() const { return _extractFlowRate; }

double OilChangeTracker::voltage() const { return _voltage; }

double OilChangeTracker::oilTemperature() const { return _oilTemperature; }

int OilChangeTracker::fillLPSState() const { return _fillLPSState; }

int OilChangeTracker::extractLPSState() const { return _extractLPSState; }

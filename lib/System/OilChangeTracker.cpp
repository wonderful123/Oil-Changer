// OilChangeTracker.cpp
#include "OilChangeTracker.h"
#include "EventManager/EventManager.h"
#include "Logger.h"
#include "SystemFactory.h"

OilChangeTracker::OilChangeTracker(std::shared_ptr<EventManager> eventManager)
    : _fillCapacity(4.0f), _amountFilled(0), _amountExtracted(0),
      _flowRateFill(0), _flowRateExtract(0), _voltage(0),
      _eventManager(eventManager) {}

void OilChangeTracker::initialize() {
  _eventManager->subscribe(shared_from_this(), Event::OilChangeTracker);
}

// Define the setter methods
void OilChangeTracker::setAmountFilled(double amount) {
  _amountFilled = amount;
}

void OilChangeTracker::setAmountExtracted(double amount) {
  _amountExtracted = amount;
}

void OilChangeTracker::setFlowRateFill(double rate) { _flowRateFill = rate; }

void OilChangeTracker::setFlowRateExtract(double rate) {
  _flowRateExtract = rate;
}

void OilChangeTracker::setVoltage(double volt) { _voltage = volt; }

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

double OilChangeTracker::flowRateFill() const { return _flowRateFill; }

double OilChangeTracker::flowRateExtract() const { return _flowRateExtract; }

double OilChangeTracker::voltage() const { return _voltage; }

// Define the additional functionality
double OilChangeTracker::remainingCapacity() const {
  return _fillCapacity - _amountFilled;
}

bool OilChangeTracker::isFull() const { return _amountFilled >= _fillCapacity; }

bool OilChangeTracker::isEmpty() const { return _amountFilled == 0; }

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
    }
  }
}

// Define the private member function
void OilChangeTracker::logStatus() const {
  // Implementation of logStatus
}

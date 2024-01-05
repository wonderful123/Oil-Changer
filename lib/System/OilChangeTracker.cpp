// OilChangeTracker.cpp
#include "OilChangeTracker.h"
#include "EventManager/EventManager.h"
#include "Logger.h"
#include "SystemFactory.h"

OilChangeTracker::OilChangeTracker(std::shared_ptr<EventManager> eventManager)
    : _fillCapacity(4.0f), _amountFilled(0), _amountExtracted(0),
      _flowRateFill(0), _flowRateExtract(0), _voltage(0),
      _eventManager(eventManager) {
  _eventManager->subscribe(std::shared_ptr<IEventListener>(this),
                           EventType::OIL_CHANGE_TRACKER_UPDATE);
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

void OilChangeTracker::setEventManager(
    std::shared_ptr<EventManager> eventManager) {
  _eventManager = eventManager;
}

void OilChangeTracker::onNotify(EventType eventType,
                                const EventData &eventData) {
  if (eventType == EventType::OIL_CHANGE_TRACKER_UPDATE) {
    // 'id' is used to specify what to update (e.g., fill capacity)
    if (eventData.id == "fill_capacity") {
      // Check the value to determine increment or decrement
      if (eventData.value > 0) {
        incrementFillCapacity(eventData.value);
      } else if (eventData.value < 0) {
        decrementFillCapacity(eventData.value);
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

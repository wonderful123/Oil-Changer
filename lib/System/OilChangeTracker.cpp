// OilChangeTracker.cpp

#include "OilChangeTracker.h" // Include the header file for the class
#include <algorithm>

// Define the setter methods
void OilChangeTracker::setAmountFilled(double amount) { _amountFilled = amount; }

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

// Define the observer pattern integration
void OilChangeTracker::attach(IObserver *observer) {
  _observers.push_back(observer);
}

void OilChangeTracker::detach(IObserver *observer) {
  _observers.erase(std::remove(_observers.begin(), _observers.end(), observer),
                   _observers.end());
}

void OilChangeTracker::notify(EventType eventType) {
  for (auto &observer : _observers) {
    observer->update(eventType);
  }
}

// Define the private member function
void OilChangeTracker::logStatus() const {
  // Implementation of logStatus
}

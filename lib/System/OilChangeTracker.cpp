// OilChangeTracker.cpp

#include "OilChangeTracker.h" // Include the header file for the class

OilChangeTracker::OilChangeTracker(std::shared_ptr<IMediator> mediator)
    : _mediator(std::move(mediator)), _fillCapacity(0), _amountFilled(0),
      _amountExtracted(0), _flowRateFill(0), _flowRateExtract(0), _voltage(0) {
  if (_mediator) {
    _mediator->registerForEvent(shared_from_this(),
                                EventType::OIL_CHANGE_TRACKER_UPDATE);
  }
}

std::shared_ptr<OilChangeTracker>
OilChangeTracker::getInstance(std::shared_ptr<IMediator> mediator) {
  static std::shared_ptr<OilChangeTracker> instance =
      std::shared_ptr<OilChangeTracker>(new OilChangeTracker(mediator));
  return instance;
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

void OilChangeTracker::setMediator(std::shared_ptr<IMediator> mediator) {
  _mediator = mediator;
}

void OilChangeTracker::receiveEvent(EventType eventType,
                                    const EventData *eventData) {
  switch (eventType) {
  case EventType::OIL_CHANGE_TRACKER_UPDATE:
    if (eventData) {
      // Assuming 'id' is used to specify what to update (e.g., fill capacity)
      if (eventData->id == "fill_capacity") {
        // Check the value to determine increment or decrement
        if (eventData->value > 0) {
          incrementFillCapacity(eventData->value);
        } else if (eventData->value < 0) {
          decrementFillCapacity(std::abs(eventData->value));
        }
      }
    }
    break;
    // Handle other events as needed
  }
}

void OilChangeTracker::notifyMediator(EventType eventType,
                                      const EventData *data) {
  if (_mediator) {
    _mediator->notify(this, eventType, data);
  }
}

// Define the private member function
void OilChangeTracker::logStatus() const {
  // Implementation of logStatus
}

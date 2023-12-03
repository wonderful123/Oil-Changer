#pragma once

#include <functional>
#include <string>
#include <vector>
#include "ISubject.h"

class OilChangeTracker : public ISubject {
public:
  static OilChangeTracker &getInstance() {
    static OilChangeTracker instance;
    return instance;
  }

  OilChangeTracker(OilChangeTracker const &) = delete;
  void operator=(OilChangeTracker const &) = delete;

  // Constructor
  OilChangeTracker(double fillCapacity)
      : _fillCapacity(fillCapacity), _amountFilled(0), _amountExtracted(0),
        _flowRateFill(0), _flowRateExtract(0), _voltage(0) {}

  // Setters
  void setAmountFilled(double amount);
  void setAmountExtracted(double amount);
  void setFlowRateFill(double rate);
  void setFlowRateExtract(double rate);
  void setVoltage(double volt);
  void incrementFillCapacity(double amount);
  void decrementFillCapacity(double amount);

  // Getters
  double fillCapacity() const;
  double amountFilled() const;
  double amountExtracted() const;
  double flowRateFill() const;
  double flowRateExtract() const;
  double voltage() const;

  // Additional functionality
  double remainingCapacity() const;
  bool isFull() const;
  bool isEmpty() const;

  // Observer pattern integration
  void attach(IObserver *observer);
  void detach(IObserver *observer);
  void notify(EventType eventType);

private:
  OilChangeTracker() = default;

  double _fillCapacity = 0;
  double _amountFilled = 0;
  double _amountExtracted = 0;
  double _flowRateFill = 0; // Current flow rate for filling
  double _flowRateExtract = 0; // Current flow rate for extraction
  double _voltage = 0;         // Voltage sense

  std::vector<IObserver *> _observers; // List of observer callbacks

  void logStatus() const;
};
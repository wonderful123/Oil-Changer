// OilChangeTracker.h
#pragma once

#include <cmath>
#include <functional>
#include <memory>
#include <string>

#include "EventManager/EventManager.h"
#include "EventManager/IEventListener.h"

class OilChangeTracker : public IEventListener,
                         public std::enable_shared_from_this<OilChangeTracker> {
public:
  OilChangeTracker(std::shared_ptr<EventManager> eventManager);
  void initialize();

  // Setters
  void setAmountFilled(double amount);
  void setAmountExtracted(double amount);
  void setFlowRateFill(double rate);
  void setFlowRateExtract(double rate);
  void setVoltage(double volt);
  void incrementFillCapacity(double amount = 0.1);
  void decrementFillCapacity(double amount = 0.1);

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

private:
  double _fillCapacity = 0;
  double _amountFilled = 0;
  double _amountExtracted = 0;
  double _flowRateFill = 0;    // Current flow rate for filling
  double _flowRateExtract = 0; // Current flow rate for extraction
  double _voltage = 0;         // Voltage sense

  std::shared_ptr<EventManager> _eventManager;
  void onNotify(Event event, Parameter parameter, float value) override;

  void logStatus() const;
};
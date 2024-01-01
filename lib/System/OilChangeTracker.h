#pragma once

#include <cmath>
#include <functional>
#include <memory>
#include <string>

#include "Mediator/IMediator.h"

class OilChangeTracker : public IColleague,
                         public std::enable_shared_from_this<OilChangeTracker> {
public:
  static std::shared_ptr<OilChangeTracker>
  getInstance(std::shared_ptr<IMediator> mediator = nullptr);

  OilChangeTracker(OilChangeTracker const &) = delete;
  void operator=(OilChangeTracker const &) = delete;

  void setMediator(std::shared_ptr<IMediator> mediator);
  void receiveEvent(EventType eventType, const EventData *eventData) override;

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
  OilChangeTracker(std::shared_ptr<IMediator> mediator);

  void initialize(std::shared_ptr<IMediator> mediator);

  double _fillCapacity = 0;
  double _amountFilled = 0;
  double _amountExtracted = 0;
  double _flowRateFill = 0;    // Current flow rate for filling
  double _flowRateExtract = 0; // Current flow rate for extraction
  double _voltage = 0;         // Voltage sense

  std::shared_ptr<IMediator> _mediator;

  void notifyMediator(EventType eventType, const EventData *data = nullptr);

  void logStatus() const;
};
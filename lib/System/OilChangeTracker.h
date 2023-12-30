#pragma once

#include <functional>
#include <memory>
#include <string>

#include "ConfigManager.h"
#include "Mediator/IMediator.h"

class OilChangeTracker : public IColleague {
public:
  OilChangeTracker(std::shared_ptr<IMediator> mediator);

  static OilChangeTracker &
  getInstance(std::shared_ptr<IMediator> mediator = nullptr);

  OilChangeTracker(OilChangeTracker const &) = delete;
  void operator=(OilChangeTracker const &) = delete;

  void setMediator(std::shared_ptr<IMediator> mediator);
  virtual void receiveEvent(EventType eventType,
                            const EventData *eventData) override;

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

private:
  double _fillCapacity = 0;
  double _amountFilled = 0;
  double _amountExtracted = 0;
  double _flowRateFill = 0;    // Current flow rate for filling
  double _flowRateExtract = 0; // Current flow rate for extraction
  double _voltage = 0;         // Voltage sense

  std::shared_ptr<IMediator> _mediator;
  std::shared_ptr<ConfigManager> _configManager;

  void notifyMediator(EventType eventType);

  void logStatus() const;
};
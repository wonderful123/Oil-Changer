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
  struct TrackerData {
    double fillCapacity;
    double amountFilled;
    double amountExtracted;
    double fillFlowRate;
    double extractFlowRate;
    double voltage;
    double oilTemperature;
    int fillLPSState;
    int extractLPSState;
    std::string currentState;
  };

  OilChangeTracker(std::shared_ptr<EventManager> eventManager);
  void initialize();

  TrackerData getCurrentData() const; // For display manager

  void reset();

  // Setters
  void incrementAmountFilled(double amount);
  void incrementAmountExtracted(double amount);
  void resetAmountFilled();
  void resetAmountExtracted();
  void setAmountFilled(double amount);
  void setAmountExtracted(double amount);
  void setFillFlowRate(double rate);
  void setExtractFlowRate(double rate);
  void incrementFillCapacity(double amount = 0.1);
  void setFillLPSState(int state);
  void setExtractLPSState(int state);
  void setVoltage(double volt);
  void setOilTemperature(double temperature);
  void setCurrentState(const std::string &state);
  void setCurrentState(Parameter parameter);

  // Getters
  double fillCapacity() const;
  double amountFilled() const;
  double amountExtracted() const;
  double fillFlowRate() const;
  double extractFlowRate() const;
  double voltage() const;
  double oilTemperature() const;
  int fillLPSState() const;
  int extractLPSState() const;

private:
  double _fillCapacity = 0;
  double _amountFilled = 0;
  double _amountExtracted = 0;
  double _fillFlowRate = 0;    // Current flow rate for filling
  double _extractFlowRate = 0; // Current flow rate for extraction
  double _voltage = 0;         // Voltage sense
  double _oilTemperature = 0;
  int _fillLPSState = 0; // Current state of the Low Pressure Switch
  int _extractLPSState = 0;
  std::string _currentState;

  std::shared_ptr<EventManager> _eventManager;
  void onNotify(Event event, Parameter parameter, float value) override;
};
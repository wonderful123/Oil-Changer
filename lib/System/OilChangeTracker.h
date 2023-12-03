#include <functional>
#include <string>

class OilChangeTracker : public ISubject {
public:
  // Constructor
  OilChangeTracker(double fillCapacity)
      : fillCapacity(fillCapacity), amountFilled(0), amountExtracted(0),
        flowRateFill(0), flowRateExtract(0), voltage(0) {}

  // Setters
  void setAmountFilled(double amount);
  void setAmountExtracted(double amount);
  void setFlowRateFill(double rate);
  void setFlowRateExtract(double rate);
  void setVoltage(double volt);

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
  double _fillCapacity;
  double _amountFilled;
  double _amountExtracted;
  double _flowRateFill;    // Current flow rate for filling
  double _flowRateExtract; // Current flow rate for extraction
  double _voltage;         // Voltage sense

  std::vector<IObserver *> _observers; // List of observer callbacks

  void logStatus() const;
};
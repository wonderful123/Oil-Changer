// DisplayManager.h
#pragma once

#include "EventManager/EventManager.h"
#include "IDisplay.h"
#include "OilChangeTracker.h"
#include <chrono>
#include <map>
#include <memory>

class DisplayManager : public IEventListener,
                       public std::enable_shared_from_this<DisplayManager> {
public:
  DisplayManager(std::shared_ptr<EventManager> eventManager,
                 std::shared_ptr<OilChangeTracker> oilChangeTracker,
                 std::map<std::string, std::shared_ptr<IDisplay>> &displays);
  ~DisplayManager();

  void initialize();
  void onNotify(Event type, Parameter parameter,
                const std::string &message) override;
  void update();

private:
  // Threshold for determining if a changed value is significant and should be
  // sent to the display
  static constexpr double CHANGE_THRESHOLD = 0.1;

  std::shared_ptr<EventManager> _eventManager;
  std::shared_ptr<OilChangeTracker> _oilChangeTracker;
  std::shared_ptr<IDisplay> _primaryDisplay;
  std::shared_ptr<IDisplay> _secondaryDisplay;
  std::map<std::string, std::shared_ptr<IDisplay>> _displays;

  OilChangeTracker::TrackerData _lastData;

  // Throttling related variables
  std::chrono::steady_clock::time_point _lastUpdate;
  const std::chrono::milliseconds _updateInterval{
      500}; // Update interval in milliseconds

  unsigned int calculateChecksum(const std::string &message);
  std::string convertLPSState(int state);
  bool hasSignificantChange(double oldValue, double newValue,
                            double threshold = CHANGE_THRESHOLD);

  bool appendChange(std::stringstream &ss, const std::string &key,
                    double oldValue, double newValue,
                    bool forceImmediate = false);
  // Overload for string data
  bool appendChange(std::stringstream &ss, const std::string &key,
                    const std::string &oldValue, const std::string &newValue,
                    bool forceImmediate = false);
  bool appendStateChange(std::stringstream &ss, const std::string &key,
                         int oldValue, int newValue);
};
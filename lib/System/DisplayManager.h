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

  std::string formatDisplayMessage(const OilChangeTracker::TrackerData &data);
  unsigned int calculateChecksum(const std::string &message);
  std::string convertLPSState(int state);
};
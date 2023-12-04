#include "IDisplay.h"
#include <vector>

class DisplayManager {
private:
  std::vector<std::unique_ptr<IDisplay>> displays;

public:
  // Add a new display to the manager
  void addDisplay(std::unique_ptr<IDisplay> display) {
    displays.push_back(std::move(display));
  }

  // Initialize all displays
  void initializeDisplays() {
    for (auto &display : displays) {
      display->initialize();
    }
  }

  // Shutdown all displays
  void shutdownDisplays() {
    for (auto &display : displays) {
      display->shutdown();
    }
  }

  // Update all displays with the same data
  void updateDisplays(const std::string &data) {
    for (auto &display : displays) {
      display->displayData(data);
    }
  }
};

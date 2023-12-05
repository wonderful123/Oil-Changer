#include "IColleague.h"
#include "IMediator.h"
#include <vector>

class ConcreteMediator : public IMediator {
  std::vector<IColleague *> colleagues;

public:
  void registerColleague(IColleague *colleague) {
    if (std::find(colleagues.begin(), colleagues.end(), colleague) ==
        colleagues.end()) {
      colleagues.push_back(colleague);
    }
  }

  void deregisterColleague(IColleague *colleague) {
    colleagues.erase(
        std::remove(colleagues.begin(), colleagues.end(), colleague),
        colleagues.end());
  }

  void notify(const IColleague *sender, EventType eventType,
              const std::string &buttonId = "") override {
    for (auto &colleague : colleagues) {
      if (colleague != sender) {
        // Additional logic for handling ButtonController events
        if (eventType == EventType::BUTTON_PRESSED) {
          handleButtonPressEvent(sender, buttonId);
        } else {
          // General event handling
          colleague->receiveEvent(eventType);
        }
      }
    }
  }

private:
  void handleButtonPressEvent(const IColleague *sender,
                              const std::string &buttonId) {
    // Implement logic specific to button press events
    // This could involve notifying specific colleagues or performing additional
    // actions
  }
};
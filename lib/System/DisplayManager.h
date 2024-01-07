#include "EventManager/EventManager.h"
#include "IDisplayAdapter.h"
#include <memory>

class DisplayManager : public IEventListener,
                       public std::enable_shared_from_this<DisplayManager> {
public:
  DisplayManager(std::shared_ptr<EventManager> eventManager,
                 std::shared_ptr<IDisplayAdapter> displayAdapter);
  ~DisplayManager();

  void onNotify(Event type, Parameter parameter, float value) override;

private:
  std::shared_ptr<EventManager> _eventManager;
  std::shared_ptr<IDisplayAdapter> _displayAdapter;
};
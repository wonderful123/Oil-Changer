// BuzzerManager.h
#pragma once

#include "EventManager/IEventListener.h"
#include <functional>
#include <memory>

class InteractionSettings;
class IBuzzer;
class EventManager;

class BuzzerManager : public IEventListener,
                      public std::enable_shared_from_this<BuzzerManager> {
public:
  BuzzerManager(std::shared_ptr<IBuzzer> buzzer,
                std::shared_ptr<InteractionSettings> &settings,
                std::shared_ptr<EventManager> eventManager);

  void initialize();
  void triggerBuzzer(int frequency, int duration);
  void beep();
  void rapidBeep();
  void doubleBeep();
  void stop();
  void updateSettings(std::shared_ptr<InteractionSettings> &settings);
  void setOnRapidBeepCallback(std::function<void()> callback);

private:
  std::shared_ptr<IBuzzer> _buzzer;
  std::shared_ptr<EventManager> _eventManager;

  void onNotify(Event type, Parameter parameter) override;
};

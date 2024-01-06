#pragma once

#include "EventManager/IEventListener.h"
#include <functional>
#include <memory>

class InteractionSettings;
class IBuzzer;

class BuzzerManager : public IEventListener {
public:
  BuzzerManager(std::shared_ptr<IBuzzer> buzzer,
                std::shared_ptr<InteractionSettings> &settings);

  void triggerBuzzer(int frequency, int duration);
  void beep();
  void rapidBeep();
  void doubleBeep();
  void stop();
  void updateSettings(std::shared_ptr<InteractionSettings> &settings);
  void setOnRapidBeepCallback(std::function<void()> callback);

private:
  std::shared_ptr<IBuzzer> _buzzer;

  void onNotify(Event type, Parameter parameter) override;
};

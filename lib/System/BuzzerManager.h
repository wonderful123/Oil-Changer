#pragma once

#include <memory>

#include "IBuzzer.h"
#include "Observer/IObserver.h"

class BuzzerManager : public IObserver { // Inherit from IObserver
public:
  BuzzerManager(std::shared_ptr<IBuzzer> buzzer,
                std::shared_ptr<InteractionSettings> &settings);

  void triggerBuzzer(int frequency, int duration);
  void startRapidBeep();
  void updateSettings(std::shared_ptr<InteractionSettings> &settings);
  void onNotify(const std::string &event, const std::string &buttonId)
      override; // Implement onNotify from IObserver
  void setOnRapidBeepCallback(std::function<void()> callback);

private:
  std::shared_ptr<IBuzzer> _buzzer;
};

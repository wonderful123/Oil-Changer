// BuzzerManager.h
#pragma once

#include <memory>

#include "IBuzzer.h"
#include "Mediator/IColleague.h"

class BuzzerManager : public IColleague {
 public:
  BuzzerManager(std::shared_ptr<IMediator> mediator,
                std::shared_ptr<IBuzzer> buzzer,
                const InteractionSettings& settings);

  void triggerBuzzer(int frequency, int duration);
  void updateSettings(const InteractionSettings& settings);
  void receiveEvent(EventType eventType, const EventData* eventData) override;

 private:
  std::shared_ptr<IBuzzer> _buzzer;
  InteractionSettings _settings;  // Store the current settings if needed
};

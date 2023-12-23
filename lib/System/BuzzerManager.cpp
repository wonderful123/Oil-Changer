// BuzzerManager.cpp
#include "BuzzerManager.h"

BuzzerManager::BuzzerManager(std::shared_ptr<IMediator> mediator,
                             std::shared_ptr<IBuzzer> buzzer,
                             const InteractionSettings& settings)
    : IColleague(mediator), _buzzer(buzzer) {
  updateSettings(settings);
}

void BuzzerManager::triggerBuzzer(int frequency, int duration) {
  if (_buzzer) {
    _buzzer->beep(frequency, duration);
  }
}

void BuzzerManager::updateSettings(const InteractionSettings& settings) {
  if (_buzzer) {
    _buzzer->updateSettings(settings);
  } else {
    Logger::error(
        "[BuzzerManager] Buzzer component is not set or initialized.");
  }
}

void BuzzerManager::receiveEvent(EventType eventType,
                                 const EventData* eventData) {
  // Handle events if necessary, potentially triggering the buzzer in response
  if (eventType == EventType::BUTTON_PRESSED) {
    // Trigger or update buzzer based on the event
  }
}

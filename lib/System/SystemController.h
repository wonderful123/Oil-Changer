// src/controllers/SystemController.h
#pragma once
#include "FSM/StateMachine.h"
#include "HardwareManager.h"
#include "IButtonControllerObserver.h" // Interface for observing button controller

class SystemController : public IButtonControllerObserver {
public:
  SystemController(std::shared_ptr<HardwareManager> hardwareManager);

  // Initialize system components
  void initialize();

  // Overrides from IButtonControllerObserver
  void onButtonPress(int buttonId) override;

  // Other system-related functions
  void startSystem();
  void stopSystem();

  // Handling FSM events
  void handleEvent(const Event &event);

private:
  std::shared_ptr<HardwareManager> _hardwareManager;
  StateMachine _stateMachine;

  // Add other members as needed

  // Private methods for internal operations
  void changeSystemState(); // Example method to change state

  // Method to register as observer to ButtonController
  void registerAsButtonObserver();
};
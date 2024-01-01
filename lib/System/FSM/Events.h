#pragma once

#include <string>
#include <tinyfsm.hpp>

// Base Button Press Event
struct ButtonPressEvent : tinyfsm::Event {
  std::string buttonId;
  explicit ButtonPressEvent(const std::string &id) : buttonId(id) {}
};

// Specific Button Press Events
struct ButtonPlusPressedEvent : ButtonPressEvent {};
struct ButtonMinusPressedEvent : ButtonPressEvent {};
struct StartButtonPressedEvent : ButtonPressEvent {};
struct ManualFillButtonPressedEvent : ButtonPressEvent {};
struct ManualExtractButtonPressedEvent : ButtonPressEvent {};

// Other Event Types
struct InitializationEvent : tinyfsm::Event {
  virtual ~InitializationEvent() = default;
};  // Base event for initialization events
struct ConfigLoadedEvent : InitializationEvent {};
struct HardwareInitializedEvent : InitializationEvent {};
struct DisplayInitializedEvent : InitializationEvent {};
struct WebServerStartedEvent : InitializationEvent {};
struct InitializationCompleteEvent : InitializationEvent {};

struct OilCapacityEvent : tinyfsm::Event {};  // Base event for oil capacity
struct OilCapacityUpdatedEvent : OilCapacityEvent {};

struct OilExtractionEvent : tinyfsm::Event {
  virtual ~OilExtractionEvent() = default;
};  // Base event for oil extraction
struct OilCapacityThresholdReachedEvent : OilExtractionEvent {};
struct ExtractLowPressureSwitchTriggeredEvent : OilExtractionEvent {};

struct InterimEvent : tinyfsm::Event {
  virtual ~InterimEvent() = default;
};

struct OilFillingEvent : tinyfsm::Event {
  virtual ~OilFillingEvent() = default;
};  // Base event for oil filling
struct OilCapacityTargetReachedEvent : OilFillingEvent {};
struct FillLowPressureSwitchTriggeredEvent : OilFillingEvent {};

// Configuration Mode Events
struct ConfigurationModeEvent : tinyfsm::Event {
  virtual ~ConfigurationModeEvent() = default;
};
struct SettingUpdatedEvent : ConfigurationModeEvent {};

// Additional Events
struct ErrorDetectedEvent : tinyfsm::Event {};
struct UserCancelledEvent : tinyfsm::Event {};
struct TimeoutEvent : tinyfsm::Event {};
struct MaintenanceNeededEvent : tinyfsm::Event {};

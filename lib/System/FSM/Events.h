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
struct ReadyEvent : InitializationEvent {};

struct OilCapacityEvent : tinyfsm::Event {};  // Base event for oil capacity
struct OilCapacityUpdatedEvent : OilCapacityEvent {};

struct OilExtractionEvent : tinyfsm::Event {
  virtual ~OilExtractionEvent() = default;
};  // Base event for oil extraction
struct OilExtractionStartedEvent : OilExtractionEvent {};
struct OilCapacityThresholdReachedEvent : OilExtractionEvent {};
struct ExtractLowPressureSwitchTriggeredEvent : OilExtractionEvent {};
struct OilExtractionCompletedEvent : OilExtractionEvent {};

struct InterimEvent : tinyfsm::Event {
  virtual ~InterimEvent() = default;
};  // Base event for filter replacement
struct InterimPeriodStartedEvent : InterimEvent {};
struct InterimPeriodFinishedEvent : InterimEvent {};

struct OilFillingEvent : tinyfsm::Event {
  virtual ~OilFillingEvent() = default;
};  // Base event for oil filling
struct OilFillingStartedEvent : OilFillingEvent {};
struct OilCapacityTargetReachedEvent : OilFillingEvent {};
struct FillLowPressureSwitchTriggeredEvent : OilFillingEvent {};
struct OilFillingCompletedEvent : OilFillingEvent {};

// Configuration Mode Events
struct ConfigurationModeEvent : tinyfsm::Event {
  virtual ~ConfigurationModeEvent() = default;
};  // Base event for configuration mode
struct ConfigModeEnteredEvent : ConfigurationModeEvent {};
struct SettingUpdatedEvent : ConfigurationModeEvent {};
struct ConfigModeExitedEvent : ConfigurationModeEvent {};

// Additional Events
struct ErrorDetectedEvent : tinyfsm::Event {};
struct UserCancelledEvent : tinyfsm::Event {};
struct TimeoutEvent : tinyfsm::Event {};
struct MaintenanceNeededEvent : tinyfsm::Event {};

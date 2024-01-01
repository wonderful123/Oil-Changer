#pragma once

#include <string>
#include <tinyfsm.hpp>

// Base Button Press Event
struct ButtonPressEvent : tinyfsm::Event {
  enum class Action {
    PRESS,
    RELEASE
  };

  enum class Id {
    PLUS,
    MINUS,
    START,
    STOP,
    MANUALEXTRACT,
    MANUALFILL
  };

  Action action;
  Id id;
};

using Action = ButtonPressEvent::Action; // Make Action available directly
using Id = ButtonPressEvent::Id;         // Make Id available directly

// Specific Button Press Events
struct PlusButtonEvent : ButtonPressEvent {};
struct MinusButtonEvent : ButtonPressEvent {};
struct StartButtonEvent : ButtonPressEvent {};
struct StopButtonEvent : ButtonPressEvent {};
struct ManualFillButtonEvent : ButtonPressEvent {};
struct ManualExtractButtonEvent : ButtonPressEvent {};

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

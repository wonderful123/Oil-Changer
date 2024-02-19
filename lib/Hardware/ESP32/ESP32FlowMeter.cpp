// ESP32FlowMeter.cpp

#ifdef PLATFORM_ESP32

#include "ESP32FlowMeter.h"
#include "Logger.h"
#include <Arduino.h>
#include <iostream>
#include <map>

ESP32FlowMeter::ESP32FlowMeter(const HardwarePinConfig &config)
    : FlowMeterBase(config), _lastUpdateTime(millis()) {
  _minUpdateIntervalMs = config.getOptionAs<int>("minUpdateIntervalMs");
  if (initPcnt(config) != Error::OK) {
    Logger::error("[ESP32FlowMeter] Failed to initialize PCNT unit correctly");
  }
  setInitialized(true);
}

ESP32FlowMeter::~ESP32FlowMeter() {
  pcnt_counter_pause(_pcntUnit);
  pcnt_counter_clear(_pcntUnit);
}

Error ESP32FlowMeter::initPcnt(const HardwarePinConfig &config) {
  static const std::map<std::string, pcnt_unit_t> pcntUnitMap = {
      {"PCNT_UNIT_0", PCNT_UNIT_0},
      {"PCNT_UNIT_1", PCNT_UNIT_1},
      {"PCNT_UNIT_2", PCNT_UNIT_2},
      {"PCNT_UNIT_3", PCNT_UNIT_3}};

  auto it = pcntUnitMap.find(config.getOptionAs<std::string>("pcntUnit"));
  if (it == pcntUnitMap.end()) {
    Logger::error("[ESP32FlowMeter] Invalid PCNT unit specified");
    return Error(Error::FlowMeterInitErrorNoPCNTUnitSpecified);
  }
  _pcntUnit = it->second;

  pcnt_config_t pcntConfig = {
      .pulse_gpio_num = config.pinNumber,
      .ctrl_gpio_num = PCNT_PIN_NOT_USED,
      .lctrl_mode = PCNT_MODE_KEEP, // Reverse counting direction if low
      .hctrl_mode = PCNT_MODE_KEEP, // Keep the primary counter mode if high
      .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
      .neg_mode = PCNT_COUNT_DIS, // Keep the counter value on the negative edge
      .counter_h_lim = 10000,
      .counter_l_lim = 0,
      .unit = _pcntUnit,
      .channel = PCNT_CHANNEL_0,
  };

  pcnt_unit_config(&pcntConfig);

  // Set the filter value (in APB_CLK cycles)
  uint16_t filterVal = config.getOptionAs<int>("pcntFilterValue");
  pcnt_set_filter_value(_pcntUnit, filterVal);
  bool enableFilter = config.getOptionAs<bool>("pcntFilterEnabled");
  if (enableFilter) {
    pcnt_filter_enable(_pcntUnit);
  } else {
    pcnt_filter_disable(_pcntUnit);
  }

  resetPcntCounter();

  return Error(Error::OK);
}

void ESP32FlowMeter::resetPcntCounter() {
  pcnt_counter_pause(_pcntUnit);
  pcnt_counter_clear(_pcntUnit);
  pcnt_counter_resume(_pcntUnit);
}

void ESP32FlowMeter::update() {
  auto currentTime = millis();
  auto timeElapsed = currentTime - _lastUpdateTime;

  if (timeElapsed >= _minUpdateIntervalMs) {
    int16_t count = getPulseCount();
    double pulsesPerSecond = count / (timeElapsed / 1000.0);
    _flowRate = pulsesPerSecond / _pulsesPerLiter;

    _totalVolume += count / _pulsesPerLiter;

    pcnt_counter_clear(_pcntUnit); // Reset the hardware pulse counter
    _lastUpdateTime = currentTime;
  }
}

void ESP32FlowMeter::reset() {
  resetPcntCounter();
  FlowMeterBase::reset(); // Call base class reset to reset
                          // flowRate and other properties
  _lastUpdateTime = millis();
}

int16_t ESP32FlowMeter::getPulseCount() const {
  int16_t count;
  pcnt_get_counter_value(_pcntUnit, &count);
  return count;
}

#endif // PLATFORM_ESP32
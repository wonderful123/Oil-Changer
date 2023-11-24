#include "FlowMeter.h"
#include "esp_log.h"

FlowMeter::FlowMeter(const GpioPinConfig &config)
    : FlowMeterBase(config), _pulseCount(0) {
  if (config.options.contains("pcntUnit")) {
    _pcntUnit = config.options["pcntUnit"].as<pcnt_unit_t>();
    initPcnt(_pcntUnit);
  } else {
    // Handle the case where the PCNT unit is not specified
    Error(Error::FlowMeterInitErrorNoPCNTUnitSpecified);
  }
}

FlowMeter::~FlowMeter() {
  pcnt_counter_pause(_pcntUnit);
  pcnt_counter_clear(_pcntUnit);
}

void FlowMeter::initPcnt(pcnt_unit_t pcntUnit) {
  pcnt_config_t pcntConfig = {
      .pulse_gpio_num = config.pinNumber,
      .ctrl_gpio_num = PCNT_PIN_NOT_USED,
      .channel = PCNT_CHANNEL_0,
      .unit = pcntUnit,
      .pos_mode = PCNT_COUNT_INC, // Count up on the positive edge
      .neg_mode = PCNT_COUNT_DIS, // Keep the counter value on the negative edge
      .lctrl_mode = PCNT_MODE_KEEP, // Reverse counting direction if low
      .hctrl_mode = PCNT_MODE_KEEP, // Keep the primary counter mode if high
      .counter_h_lim = 10000,
      .counter_l_lim = 0,
  };

  pcnt_unit_config(&pcntConfig);
  pcnt_counter_pause(pcntUnit);
  pcnt_counter_clear(pcntUnit);
  pcnt_counter_resume(pcntUnit);
}

double FlowMeter::getFlowRate() const {
  // Implement flow rate calculation based on _pulseCount
  auto count = getPulseCount();
  // Use 'count' for flow rate calculation
  return flowRate; // Calculated value
}

void FlowMeter::reset() {
  pcnt_counter_pause(_pcntUnit);
  pcnt_counter_clear(_pcntUnit);
  pcnt_counter_resume(_pcntUnit);
  FlowMeterBase::reset(); // Call base class reset to reset flowRate and other
                          // properties
}

int16_t FlowMeter::getPulseCount() const {
  int16_t count;
  pcnt_get_counter_value(_pcntUnit, &count);
  return count;
}
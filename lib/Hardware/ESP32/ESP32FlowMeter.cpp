#ifdef PLATFORM_ESP32

#include "ESP32FlowMeter.h"
#include "driver/pcnt.h"
#include "Logger.h"
#include <iostream>
ESP32FlowMeter::ESP32FlowMeter(const HardwarePinConfig &config)
    : FlowMeterBase(config), _pulseCount(0) {

  pcnt_unit_t pcntUnit;
  std::string configPcnt = config.getOptionAs<std::string>("pcntUnit");
  if (configPcnt == "PCNT_UNIT_0")
    pcntUnit = PCNT_UNIT_0;
  else if (configPcnt == "PCNT_UNIT_1")
    pcntUnit = PCNT_UNIT_1;
  else if (configPcnt == "PCNT_UNIT_2")
    pcntUnit = PCNT_UNIT_2;
  else if (configPcnt == "PCNT_UNIT_3")
    pcntUnit = PCNT_UNIT_3;
  else {
    Error(Error::FlowMeterInitErrorNoPCNTUnitSpecified);
    setInitialized(true);
  }

  pcnt_config_t pcntConfig = {
      .pulse_gpio_num = config.pinNumber,
      .ctrl_gpio_num = PCNT_PIN_NOT_USED,
      .lctrl_mode = PCNT_MODE_KEEP, // Reverse counting direction if low
      .hctrl_mode = PCNT_MODE_KEEP, // Keep the primary counter mode if high
      .pos_mode = PCNT_COUNT_INC,   // Count up on the positive edge
      .neg_mode = PCNT_COUNT_DIS, // Keep the counter value on the negative edge
      .counter_h_lim = 10000,
      .counter_l_lim = 0,
      .unit = pcntUnit,
      .channel = PCNT_CHANNEL_0,
  };

  pcnt_unit_config(&pcntConfig);
  pcnt_counter_pause(pcntUnit);
  pcnt_counter_clear(pcntUnit);
  pcnt_counter_resume(pcntUnit);
}

ESP32FlowMeter::~ESP32FlowMeter() {
  pcnt_counter_pause(_pcntUnit);
  pcnt_counter_clear(_pcntUnit);
}

double ESP32FlowMeter::getFlowRate() const {
  // Implement flow rate calculation based on _pulseCount
  auto count = getPulseCount();
  // Use 'count' for flow rate calculation
  return flowRate; // Calculated value
}

void ESP32FlowMeter::reset() {
  pcnt_counter_pause(_pcntUnit);
  pcnt_counter_clear(_pcntUnit);
  pcnt_counter_resume(_pcntUnit);
  FlowMeterBase::reset(); // Call base class reset to reset flowRate and other
                          // properties
}

int16_t ESP32FlowMeter::getPulseCount() const {
  int16_t count;
  pcnt_get_counter_value(_pcntUnit, &count);
  return count;
}

#endif // PLATFORM_ESP32
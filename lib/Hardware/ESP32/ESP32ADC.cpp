#ifdef PLATFORM_ESP32

#include "ESP32ADC.h"
#include "Logger.h"

ESP32ADC::ESP32ADC(const HardwarePinConfig &config)
    : ADCBase(config) {
    configureAttenuation(config);
    setInitialized(true); // Mark ADC as initialized
}

int ESP32ADC::read() const {
    return analogRead(_pinNumber);
}

void ESP32ADC::configureAttenuation(const HardwarePinConfig &config) {
    int attenuationValue = config.getOptionAs<int>("attenuation");
    if (attenuationValue >= 0 && attenuationValue <= 3) {
      _attenuation = static_cast<adc_attenuation_t>(attenuationValue);
      analogSetAttenuation(_attenuation);
      Logger::info("[ESP32ADC] ADC attenuation set to " +
                   std::to_string(attenuationValue));
    } else {
        Error(ADCInvalidAttenuationValue);
        Logger::error("[ESP32ADC] Invalid ADC attenuation value on pin: " + std::to_string(config.pinNumber));
        setInitialized(false);
    }
}
#endif // PLATFORM_ESP32

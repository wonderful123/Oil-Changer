#include "ADC.h"
#include "Logger.h"

#ifdef PLATFORM_ESP32

ADC::ADC(const GpioPinConfig &config)
    : ADCBase(config) {
    configureAttenuation(config);
    setInitialized(true); // Mark ADC as initialized
    Logger::info("ADC initialized on pin: " + std::to_string(config.pinNumber));
}

int ADC::read() const {
    return analogRead(_pinNumber);
}

void ADC::configureAttenuation(const GpioPinConfig &config) {
    int attenuationValue = config.getOptionAs<int>("attenuation");
    if (attenuationValue >= 0 && attenuationValue <= 3) {
        _attenuation = static_cast<adc_atten_t>(attenuationValue);
        adc1_config_channel_atten(static_cast<adc1_channel_t>(_pinNumber), _attenuation);
        Logger::info("ADC attenuation set to " + std::to_string(attenuationValue));
    } else {
        Error(ADCInvalidAttenuationValue);
        Logger::error("Invalid ADC attenuation value on pin: " + std::to_string(config.pinNumber));
        setInitialized(false);
    }
}
#endif // PLATFORM_ESP32

#include "ESP32Serial.h"

#ifdef PLATFORM_ESP32

ESP32Serial::ESP32Serial(const HardwarePinConfig &config)
    : _serial(0),               // Default to UART0. Adjust as needed.
      _rxPin(config.pinNumber), // Assuming pinNumber is RX
      _txPin(-1) {              // TX pin not specified in this example
  // Here, you can extract and use additional config details as needed
}

void ESP32Serial::begin(unsigned long baudrate) {
  if (_txPin > -1) {
    _serial.begin(baudrate, SERIAL_8N1, _rxPin, _txPin);
  } else {
    _serial.begin(baudrate, SERIAL_8N1, _rxPin);
  }
}

void ESP32Serial::end() { _serial.end(); }

size_t ESP32Serial::write(uint8_t byte) { return _serial.write(byte); }

int ESP32Serial::read() { return _serial.read(); }

int ESP32Serial::available() { return _serial.available(); }

void ESP32Serial::flush() { _serial.flush(); }

#endif // PLATFORM_ESP32

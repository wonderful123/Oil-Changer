#ifdef PLATFORM_ESP32

#include "ESP32Serial.h"
#include "Error.h"

ESP32Serial::ESP32Serial(const HardwarePinConfig &config)
    : ISerial(config), _serial(0), // Default to UART0. Adjust as needed.
      _rxPin(-1),               // Default values in case they are not provided
      _txPin(-1) { // Default baud rate

  // Determine UART port from the configuration
  int uartPort = 0; // Default to UART0
  auto portIt = config.options.find("uartPort");
  if (portIt != config.options.end()) {
    // Convert port number from string to integer
    char *end;
    long portNum = strtol(portIt->second.c_str(), &end, 10);
    if (*end != '\0' || portNum < 0 || portNum > 2) { // Invalid or out of range
      Error(Error::SerialInvalidUartPortNumber);
      return;
    }
    uartPortNumber = static_cast<int>(portNum);
    _serial = HardwareSerial(uartPortNumber);
  }

  // Checking if pins are provided in the configuration
  auto rxIt = config.pins.find("RXD");
  if (rxIt != config.pins.end()) {
    _rxPin = rxIt->second;
  }

  auto txIt = config.pins.find("TXD");
  if (txIt != config.pins.end()) {
    _txPin = txIt->second;
  }

  // Extracting the baud rate if provided
  auto baudIt = config.options.find("baud");
  if (baudIt != config.options.end()) {
    std::istringstream ss(baudIt->second);
    if (!(ss >> _baud)) { // If conversion fails, default to 9600
      _baud = 9600;
    }
  }

  // Initialize the Serial with extracted configurations
  if (_rxPin != -1 && _txPin != -1) {
    begin(_baud);
    setInitialized(true);
  } else {
    Error(Error::SerialHardwareConfigError);
  }
}

void ESP32Serial::begin(unsigned long baudrate) {
  if (_txPin > -1) {
    _serial.begin(baudrate, SERIAL_8N1, _rxPin, _txPin);
  } else {
    if (!Serial) {
      Logger::error("Serial port not ready");
      return;
    }
    _serial.begin(baudrate, SERIAL_8N1, _rxPin);
  }
}

void ESP32Serial::end() { _serial.end(); }

size_t ESP32Serial::write(uint8_t byte) { return _serial.write(byte); }

int ESP32Serial::read() { return _serial.read(); }

int ESP32Serial::available() { return _serial.available(); }

void ESP32Serial::flush() { _serial.flush(); }

#endif // PLATFORM_ESP32

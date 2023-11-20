#include "Error.h"
#include "Logger.h" // Include Logger

// Initialize static members
Error::Code Error::_lastErrorCode = Error::OK;
std::mutex Error::_lastErrorCodeMutex;

// Define the static member 'ErrorMessages'
#define ERROR_CODE(code, message) {Error::code, message},
const std::array<Error::ErrorInfo, Error::NumErrors> Error::ErrorMessages = {
    {ERROR_CODE_DEFINITIONS}};
#undef ERROR_CODE

// Initialize the static logger callback
Logger::LogCallback Error::_loggerCallback = nullptr;

void Error::setLoggerCallback(Logger::LogCallback callback) {
  _loggerCallback = callback;
}

const char *Error::getErrorMessage(Code code) const {
  size_t index = static_cast<size_t>(code);
  if (index < ErrorMessages.size()) {
    return ErrorMessages[index].message;
  }
  return "Unknown error code.";
}

std::string Error::getFormattedMessage(Code code) const {
  size_t index = static_cast<size_t>(code);
  return "Error Code - " + std::to_string(index) + "::" + getErrorMessage(code);
}

void Error::notifyLogger() const {
  if (_code != OK) {
    std::string message = getFormattedMessage(_code);
    _loggerCallback(Logger::ERROR, message.c_str());

    std::lock_guard<std::mutex> lock(_lastErrorCodeMutex);
    _lastErrorCode =
        _code; // Update the last error code in a thread-safe manner
  }
}

Error::Code Error::getLastErrorCode() {
  std::lock_guard<std::mutex> lock(_lastErrorCodeMutex);
  return _lastErrorCode;
}

void Error::resetLastError() {
  std::lock_guard<std::mutex> lock(_lastErrorCodeMutex);
  _lastErrorCode = OK;
}
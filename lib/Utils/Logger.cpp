// Logger.cpp

#include "Logger.h"
#include <mutex> // Include for std::mutex

// Initialize the static logger callback
Logger::LogCallback Logger::_log_callback = nullptr;

Logger::Level Logger::_logLevel = Logger::Level::INFO;

// Define the static mutex
std::mutex Logger::_mutex;

void Logger::setLogCallback(LogCallback callback) {
  std::lock_guard<std::mutex> lock(_mutex);
  if (callback) {
    _log_callback = callback;
  }
}

void Logger::log(Logger::Level level, const std::string &message) {
  if (level < _logLevel)
    return; // Filter out messages below current level
  std::lock_guard<std::mutex> lock(_mutex);
  if (_log_callback) {
    try {
      _log_callback(level, message);
    } catch (...) {
      // Handle or ignore exceptions from the log callback
    }
  }
}

// Additional function to convert Logger Level to String
const std::string Logger::levelToString(Logger::Level level) {
  switch (level) {
  case Logger::DEBUG:
    return "DEBUG";
  case Logger::INFO:
    return "INFO";
  case Logger::WARN:
    return "WARN";
  case Logger::ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}
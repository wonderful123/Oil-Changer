#include "Logger.h"
#include <mutex> // Include for std::mutex

// Initialize the static logger callback
Logger::LogCallback Logger::_log_callback = nullptr;

// Define the static mutex
std::mutex Logger::_mutex;

void Logger::setLogCallback(LogCallback callback) {
  std::lock_guard<std::mutex> lock(_mutex);
  _log_callback = callback;
}

static void log(Level level, const char *message) {
  if (level < _logLevel)
    return; // Filter out messages below current level
  std::lock_guard<std::mutex> lock(_mutex);
  if (_log_callback) {
    _log_callback(level, message);
  }
}

// Additional function to convert Logger Level to String
const char *Logger::levelToString(Logger::Level level) {
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
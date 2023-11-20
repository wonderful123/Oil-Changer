#pragma once

#include <functional>
#include <mutex>
#include <string>

class Logger {
public:
  enum Level { DEBUG, INFO, WARN, ERROR };

  using LogCallback = std::function<void(Level, const char *)>;

  static void setLogCallback(LogCallback callback);
  static void log(Level level, const char *message);
  static const char *levelToString(Logger::Level level);

  static void debug(const std::string &message) { log(DEBUG, message.c_str()); }
  static void info(const std::string &message) { log(INFO, message.c_str()); }
  static void warn(const std::string &message) { log(WARN, message.c_str()); }
  static void error(const std::string &message) { log(ERROR, message.c_str()); }

  static void setLogLevel(Level level) { _currentLevel = level; }
  static Level getLogLevel() { return _currentLevel; }

private:
  static LogCallback _log_callback;
  static std::mutex _mutex;
  static Level _logLevel; // Current log level for filtering
};

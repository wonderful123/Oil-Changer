// Logger.h
#pragma once

#include <functional>
#include <mutex>
#include <string>

class Logger {
public:
  enum Level { DEBUG, INFO, WARN, ERROR };
  using LogCallback = std::function<void(Level, const std::string &)>;

  static void setLogCallback(LogCallback callback);
  static void log(Level level, const std::string &message);
  static const std::string levelToString(Level level);

  static void debug(const std::string &message) { log(DEBUG, message); }
  static void info(const std::string &message) { log(INFO, message); }
  static void warn(const std::string &message) { log(WARN, message); }
  static void error(const std::string &message) { log(ERROR, message); }

  static void setLogLevel(Level level) { _logLevel = level; }
  static Level getLogLevel() { return _logLevel; }

private:
  static LogCallback _log_callback;
  static std::mutex _mutex;
  static Level _logLevel;
};
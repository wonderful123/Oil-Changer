#pragma once

#include "Logger.h"
#include <array>
#include <string>

// Macro list of error codes and their messages
#define ERROR_CODE_DEFINITIONS                                                 \
  ERROR_CODE(OK, "No Error.")                                                  \
  ERROR_CODE(FileOpenFailure, "Unable to open the specified file.")            \
  ERROR_CODE(FileReadFailure, "Error encountered while reading the file.")     \
  ERROR_CODE(FileSystemMountError, "Failed to mount the file system.")         \
  ERROR_CODE(FileWriteFailure, "Error encountered while writing the file.")    \
  ERROR_CODE(HardwareConfigHardwareKeyMissing,                                 \
             "Missing 'hardware' key in hardware configuration.")              \
  ERROR_CODE(HardwareConfigGpioPinsKeyMissing,                                 \
             "Missing or undefined 'gpioPins' key in hardware configuration.") \
  ERROR_CODE(HardwareConfigGpioPinsTypeMismatch,                               \
             "The 'gpioPins' key is expected to be an array.")                 \
  ERROR_CODE(HardwareConfigGpioPinNumberMissing,                               \
             "GPIO pin number is undefined in hardware configuration.")        \
  ERROR_CODE(HardwareConfigGpioPinIdMissing,                                   \
             "Missing GPIO pin ID in hardware configuration.")                 \
  ERROR_CODE(HardwareConfigGpioPinTypeMissing,                                 \
             "GPIO pin type is not specified in hardware configuration.")      \
  ERROR_CODE(HardwareConfigAdcOptionsKeyMissing,                               \
             "'options' key for ADC pin configuration is missing.")            \
  ERROR_CODE(HardwareConfigAdcResolutionOptionMissing,                         \
             "Missing 'resolution' key in ADC 'options'.")                     \
  ERROR_CODE(JsonInputInvalid,                                                 \
             "The provided JSON input is invalid or malformed.")               \
  ERROR_CODE(JsonMemoryError,                                                  \
             "Insufficient memory to process the JSON document.")              \
  ERROR_CODE(JsonInputEmpty, "The JSON input provided is empty.")              \
  ERROR_CODE(JsonInputIncomplete,                                              \
             "The JSON input is incomplete or ends abruptly.")                 \
  ERROR_CODE(JsonDeserializationFailure,                                       \
             "Failed to deserialize the JSON document due to an error.")       \
  ERROR_CODE(ConfigTypeNotRecognized,                                          \
      "The specified configuration type is not recognized or supported.")
#undef ERROR_CODE

class Error {
public:
  enum Code {
#define ERROR_CODE(code, message) code,
    ERROR_CODE_DEFINITIONS
#undef ERROR_CODE
        NumErrors
  };

  struct ErrorInfo {
    Code code;
    const char *message;
  };

  static const std::array<ErrorInfo, NumErrors> ErrorMessages;

  Error() : _code(OK) {}
  Error(Code c) : _code(c) { notifyLogger(); }

  // Compare with Error
  friend bool operator==(const Error &lhs, const Error &rhs) {
    return lhs._code == rhs._code;
  }
  friend bool operator!=(const Error &lhs, const Error &rhs) {
    return lhs._code != rhs._code;
  }

  // Compare with Code
  friend bool operator==(const Error &lhs, Code rhs) {
    return lhs._code == rhs;
  }
  friend bool operator==(Code lhs, const Error &rhs) {
    return lhs == rhs._code;
  }
  friend bool operator!=(const Error &lhs, Code rhs) {
    return lhs._code != rhs;
  }
  friend bool operator!=(Code lhs, const Error &rhs) {
    return lhs != rhs._code;
  }

  explicit operator bool() const { return _code != OK; }

  const char *getErrorMessage(Code code) const;
  std::string getFormattedMessage(Code code) const;

  static void setLoggerCallback(Logger::LogCallback callback);

  Code code() const { return _code; }

private:
  Code _code;
  static Logger::LogCallback _loggerCallback;

  void notifyLogger() const;
};

// Static assert to ensure the number of error messages matches the
// number of error codes minus one for NumErrors.
static_assert(Error::ErrorMessages.size() ==
                  static_cast<size_t>(Error::NumErrors),
              "ErrorMessages size mismatch");
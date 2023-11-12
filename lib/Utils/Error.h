#pragma once

#include "Logger.h"
#include <array>
#include <string>

// Macro list of error codes and their messages
#define ERROR_CODE_DEFINITIONS                                                 \
  ERROR_CODE(OK, "No error.")                                                  \
  ERROR_CODE(FileSystemMountFailure, "Error mounting the file system.")        \
  ERROR_CODE(ConfigUndefinedHardwareKey,                                       \
             "The 'hardware' key is undefined in the configuration.")          \
  ERROR_CODE(ConfigUndefinedGpioPinsArrayKey,                                  \
             "The 'gpioPins' array key is undefined in the configuration.")    \
  ERROR_CODE(ConfigUndefinedGpioKeyForPin,                                     \
             "The 'gpio' key is undefined within a pin configuration.")        \
  ERROR_CODE(ConfigMissingGpioPinId,                                           \
             "The 'id' key is missing within a pin configuration.")            \
  ERROR_CODE(ConfigMissingGpioPinType,                                         \
             "The 'type' key is missing within a pin configuration.")          \
  ERROR_CODE(ConfigMissingADCOptionsKey,                                       \
             "The 'options' key is missing within an ADC pin configuration.")  \
  ERROR_CODE(ConfigMissingADCResolutionOption,                                 \
             "The 'resolution' key is missing within ADC 'options'.")          \
  ERROR_CODE(                                                                  \
      ConfigInvalidOptionType,                                                 \
      "The type of a configuration option does not match an expected type.")   \
  ERROR_CODE(                                                                  \
      ConfigRequestConfigForInvalidComponent,                                  \
      "Invalid or missing component requested in function getConfigFor.")      \
  ERROR_CODE(DuplicateComponentId,                                             \
             "Component ID should be unique in configuration.")                \
  ERROR_CODE(FileNotOpen, "The file could not be opened.")                     \
  ERROR_CODE(FileReadError, "An error occurred while reading from the file.")  \
  ERROR_CODE(JsonInvalidInput, "The JSON input is invalid.")                   \
  ERROR_CODE(                                                                  \
      JsonNoMemory,                                                            \
      "There is not enough memory to parse or manage the JSON document.")      \
  ERROR_CODE(JsonEmptyInput, "The JSON input is empty.")                       \
  ERROR_CODE(JsonIncompleteInput, "The JSON input is incomplete.")             \
  ERROR_CODE(                                                                  \
      JsonDeserializationError,                                                \
      "An error occurred during the deserialization of the JSON document.")

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
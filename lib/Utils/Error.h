// Error.h
#pragma once

#include "Logger.h"
#include <array>
#include <string>

// Macro list of error codes and their messages
#define ERROR_CODE_DEFINITIONS                                                 \
  ERROR_CODE(OK, "No Error.")                                                  \
  ERROR_CODE(FileSystemMountFailure, "Failed to mount the file system.")       \
  ERROR_CODE(FileOpenFailure, "Unable to open the specified file.")            \
  ERROR_CODE(FileReadFailure, "Error encountered while reading the file.")     \
  ERROR_CODE(FileSystemMountError, "Failed to mount the file system.")         \
  ERROR_CODE(FileWriteFailure, "Error encountered while writing the file.")    \
  ERROR_CODE(HardwareConfigComponentsKeyMissing,                               \
             "Missing 'components' key in hardware configuration.")            \
  ERROR_CODE(HardwareConfigInitError,                                          \
             "Failed to initialize the hardware configuration.")               \
  ERROR_CODE(HardwareConfigBuzzerInitError,                                    \
             "Failed to initialize the buzzer.")                               \
  ERROR_CODE(JsonInputInvalid,                                                 \
             "The provided JSON input is invalid or malformed.")               \
  ERROR_CODE(JsonMemoryError,                                                  \
             "Insufficient memory to process the JSON document.")              \
  ERROR_CODE(JsonInputEmpty, "The JSON input provided is empty.")              \
  ERROR_CODE(JsonInputIncomplete,                                              \
             "The JSON input is incomplete or ends abruptly.")                 \
  ERROR_CODE(JsonDeserializationFailure,                                       \
             "Failed to deserialize the JSON document due to an error.")       \
  ERROR_CODE(                                                                  \
      ConfigTypeNotRecognized,                                                 \
      "The specified configuration type is not recognized or supported.")      \
  ERROR_CODE(DigitalIOModeOptionMissingOrInvalid,                              \
             "Missing or invalid mode in DigitalIO options")                   \
  ERROR_CODE(ADCInvalidAttenuationValue, "Invalid ADC attenuation value.")     \
  ERROR_CODE(FlowMeterInitErrorNoPCNTUnitSpecified,                            \
             "FlowMeter: No PCNT unit specified for GPIO pin")                 \
  ERROR_CODE(InteractionSettingsButtonInteractionKeyMissing,                   \
             "Missing 'buttonInteraction' key in interactionSettings.json")    \
  ERROR_CODE(InteractionSettingsCommonSettingsKeyMissing,                      \
             "Missing 'common' key in interactionSettings.json")               \
  ERROR_CODE(InteractionSettingsButtonsSubkeyMissing,                          \
             "Missing 'buttons' subkey in interactionsSettings.json")          \
  ERROR_CODE(InteractionSettingsBeepSettingsSubkeyMissing,                     \
             "Missing 'beep' subkey in interactionSettings.json")              \
  ERROR_CODE(InteractionSettingsFeedbackSubkeyMissing,                         \
             "Missing 'feedback' subkey in interactionSettings.json")          \
  ERROR_CODE(SerialHardwareConfigError,                                        \
             "Failed to initialize the serial hardware configuration.")        \
  ERROR_CODE(SerialInvalidUartPortNumber,                                      \
             "ESP32 Serial: Invalid UART port number for ESP32.")              \
  ERROR_CODE(HardwareComponentCreationFailure,                                 \
             "Failed to create a hardware component.")                         \
  ERROR_CODE(BuzzerManagerInitializationFailure,                               \
             "Failed to initialize BuzzerManager.")                            \
  ERROR_CODE(ConfigManagerInteractionSettingsError,                            \
             "Failed to return interaction settings from config manager.")     \
  ERROR_CODE(ConfigManagerHardwareConfigNotFound,                              \
             "Failed to return hardware configuration from config manager.")   \
  ERROR_CODE(HardwareConfigButtonsNotFound,                                    \
             "No buttons found in hardware configuration.")                    \
  ERROR_CODE(FileHandlerIsNull, "FileHandler is null.")                        \
  ERROR_CODE(MotorSettingsJsonMissingMotorSettingsKey,                         \
             "MotorSettings: Missing 'motorSettings' key")                     \
  ERROR_CODE(MotorSettingsValidationMaxSpeedLessThanMinSpeed,                  \
             "MotorSettings: Max speed must be greater than min speed.")       \
  ERROR_CODE(                                                                  \
      MotorSettingsValidationMinSpeedLessThanZero,                             \
      "MotorSettings: Min speed must be greater than or equal to zero.")       \
  ERROR_CODE(MotorSettingsValidationRampingDurationLessThanZero,               \
             "MotorSettings: Ramping duration must be greater than zero.")     \
  ERROR_CODE(JsonDocSizeExceededMaximum4000BytesForESP32,                      \
             "JSON document size exceeded maximum of 4000 bytes for ESP32.")   \
  ERROR_CODE(HardwareDisplayCreationFailure,                                   \
             "Failed to create a hardware display.")                           \

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

  Error(Code c) : _code(c) {
    if (_code != OK) {
      notifyLogger();
    }
  }

  // Method to get the last error code (for testing)
  static Code getLastErrorCode ();

  // Method to reset the last error code (for testing)
  static void resetLastError();

  static std::string getErrorMessage(Code code);
  static std::string getFormattedMessage(Code code);

  static void setLoggerCallback(Logger::LogCallback callback);

  Code code() const { return _code; }

  // Compare with Error
  explicit operator bool() const { return _code != OK; }

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

private:
  Code _code;
  static Logger::LogCallback _loggerCallback;

  // Static member and mutex for thread-safe last error code
  static Code _lastErrorCode;
  static std::mutex _lastErrorCodeMutex;

  void notifyLogger() const;
};

// Static assert to ensure the number of error messages matches the
// number of error codes minus one for NumErrors.
static_assert(Error::ErrorMessages.size() ==
                  static_cast<size_t>(Error::NumErrors),
              "ErrorMessages size mismatch");
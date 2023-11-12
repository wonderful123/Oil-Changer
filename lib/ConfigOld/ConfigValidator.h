#pragma once

#include "ConfigConstants.h"
#include <ArduinoJson.h>
#include <Error.h>

namespace ConfigValidator {

// Validate the entire configuration.
Error validateConfiguration(
    const ArduinoJson::StaticJsonDocument<ConfigConstants::JSON_DOC_SIZE> &doc);
} // namespace ConfigValidator

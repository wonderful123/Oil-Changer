#include "BaseConfig.h"
#include "Error.h"

class InteractionSettingsConfig : public BaseConfig {
public:
  explicit InteractionSettingsConfig(IFileHandler *fileHandler)
      : BaseConfig(fileHandler) {}

  Error save(const std::string &filename) const override;

protected:
  Error parseJson(const DynamicJsonDocument &doc) override;

private:

};
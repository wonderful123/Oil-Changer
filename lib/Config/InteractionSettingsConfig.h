class InteractionSettingsConfig : public BaseConfig {
public:
  explicit InteractionSettingsConfig(IFileHandler *fileHandler)
      : BaseConfig(fileHandler) {}

  // Implement load and save methods
  Error load(const std::string &filename) override {
    // Code to load and parse interaction settings JSON
  }

  Error save(const std::string &filename) const override {
    // Code to save interaction settings JSON
  }

protected:
  Error parseJson(const DynamicJsonDocument &doc) override {
    // Code to parse JSON into internal data structures
  }

private:

};
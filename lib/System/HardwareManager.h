// HardwareManager.h
#pragma once

#include "Error.h"
#include "HardwareComponent.h"
#include <map>
#include <memory>

class IDisplay;
class ConfigManager;
class HardwareInitializer;

class HardwareManager {
public:
  HardwareManager(std::shared_ptr<ConfigManager> configManager);
  Error initialize();
  std::map<std::string, std::shared_ptr<IDisplay>> getDisplays();

  template <typename T>
  std::shared_ptr<T> getComponentById(const std::string &id) const {
    auto it = _components.find(id);
    if (it != _components.end()) {
      auto component = std::static_pointer_cast<T>(it->second);
      return component;
    }

    return nullptr;
  }

  template <typename T>
  std::vector<std::shared_ptr<T>> getComponentsByType(const std::string &type) {
    std::vector<std::shared_ptr<T>> componentsOfType;
    for (const auto &pair : _components) {
      auto component = pair.second;
      if (component && component->type() == type) {
        componentsOfType.push_back(std::static_pointer_cast<T>(component));
      }
    }
    return componentsOfType;
  }

private:
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<HardwareInitializer> _initializer;
  std::map<std::string, std::shared_ptr<HardwareComponent>> _components;
  std::map<std::string, std::shared_ptr<IDisplay>> _displays;
};

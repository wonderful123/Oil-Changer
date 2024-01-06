// HardwareManager.h
#pragma once

/*
HardwareManager Overview
Role and Purpose
The HardwareManager serves as the centralized hub for managing all
hardware-related interactions in the Oil Change Machine. Its primary role is to
abstract the hardware layer from the higher-level system logic, providing a
unified interface for hardware operations and ensuring a clean separation of
concerns.

*/

#include "HardwareComponent.h"
#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <vector>

class ConfigManager;
class HardwarePinConfig;
class Error;

class HardwareManager {
private:
  // Unified map to hold all types of components by id
  std::map<std::string, std::shared_ptr<HardwareComponent>> _components;

public:
  virtual Error initialize();
  virtual bool isComponentInitialized(const std::string &componentId) const;

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
};

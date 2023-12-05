#include <memory>
#include <typeindex>
#include <unordered_map>

class DIContainer {
public:
  template <typename T, typename... Args>
  static std::shared_ptr<T> resolve(Args &&...args) {
    auto typeIndex = std::type_index(typeid(T));
    auto it = instances.find(typeIndex);
    if (it == instances.end()) {
      auto instance = std::make_shared<T>(std::forward<Args>(args)...);
      instances[typeIndex] = instance;
      return instance;
    }
    return std::static_pointer_cast<T>(it->second);
  }

private:
  static std::unordered_map<std::type_index, std::shared_ptr<void>> instances;
};

// Define the static member
std::unordered_map<std::type_index, std::shared_ptr<void>>
    DIContainer::instances;
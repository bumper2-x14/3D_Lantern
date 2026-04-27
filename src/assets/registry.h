#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <unordered_map>
#include <iostream>

/// @brief Generic registry that stores objects using string keys.
///        Takes ownership of pointers and deletes them when needed.
template<typename T>
class Registry {
    public:
        /// @brief Default constructor.
        Registry() = default;

        // disable copy (we manage raw pointers)
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        /// @brief Destructor (deletes all stored objects).
        ~Registry() {
            for (auto& [key, ptr] : storage){
                if (ptr)
                    delete ptr;
            }
        }

        /// @brief Adds an object with a given key.
        /// @param key Unique name.
        /// @param target Pointer to object.
        /// @return Pointer if added, nullptr if key already exists.
        T* add(const std::string& key, T* target) {
            auto [it, added] = storage.emplace(key, target);
            if (!added) {
                std::cerr << "Registry: duplicate key '" << key << "'\n";
                delete target;
                return nullptr;
            }

            return target;
        }

        /// @brief Returns object by key.
        T* get(const std::string& key) const {
            auto it = storage.find(key);
            return it != storage.end() ? it->second : nullptr;
        }

        /// @brief Removes and deletes object by key.
        void remove(const std::string& key) {
            auto it = storage.find(key);
            if (it == storage.end()) return;
            delete it->second;
            storage.erase(it);
        }

        /// @brief Checks if key exists.
        bool has(const std::string& key) const {
            return storage.count(key) > 0;
        }

        /// @brief Returns all stored objects.
        const std::unordered_map<std::string, T*>& getAll() const { return storage; }

    private:
        std::unordered_map<std::string, T*> storage; ///< Internal storage (key -> pointer).
};

#endif
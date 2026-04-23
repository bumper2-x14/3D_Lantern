#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <unordered_map>
#include <iostream>

template<typename T>
class Registry {
    public:
        Registry() = default;

        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        ~Registry() {
            for (auto& [key, ptr] : storage){
                if (ptr)
                    delete ptr;
            }
        }

        T* add(const std::string& key, T* target) {
            auto [it, added] = storage.emplace(key, target);
            if (!added) {
                std::cerr << "Registry: duplicate key '" << key << "'\n";
                delete target;
                return nullptr;
            }

            return target;
        }

        T* get(const std::string& key) const {
            auto it = storage.find(key);
            return it != storage.end() ? it->second : nullptr;
        }

        void remove(const std::string& key) {
            auto it = storage.find(key);
            if (it == storage.end()) return;
            delete it->second;
            storage.erase(it);
        }

        bool has(const std::string& key) const {
            return storage.count(key) > 0;
        }

        const std::unordered_map<std::string, T*>& getAll() const { return storage; }

    private:
        std::unordered_map<std::string, T*> storage;
};

#endif
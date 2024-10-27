// config_mapping.cpp
#include "rns_mapping.h"

std::pair<std::string, std::string> get_config_and_hash(const std::string &identifier)
{
    static std::mutex map_mutex;
    static std::map<std::string, std::pair<std::string, std::string>> mapping = {
        {"identifier1", {"/path/to/config1", "hash1"}},
        {"identifier2", {"/path/to/config2", "hash2"}},
        // Add more mappings as needed
    };

    std::lock_guard<std::mutex> lock(map_mutex);

    auto it = mapping.find(identifier);
    if (it != mapping.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Identifier not found: " + identifier);
    }
}

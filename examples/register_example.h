#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <cctype>

struct Example
{
    std::function<int()> func;
    std::string description;
};

extern std::unordered_map<std::string, Example> examples;

inline auto& get_examples()
{
    return examples;
}

// Macro to register an example at static init time
#define REGISTER_EXAMPLE(func) \
    struct func##_registrar { \
        func##_registrar() { \
            auto& map = get_examples(); \
            std::string name = #func; \
            map[name] = { func, #func }; \
        } \
    }; \
    static func##_registrar func##_registrar_instance

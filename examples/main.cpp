#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "register_example.h"

std::unordered_map<std::string, Example> examples;

void print_usage()
{
    std::cout << "Usage: synth_demo <example_name>\n\n";
    std::cout << "Available examples:\n";
    for (const auto& [name, ex] : get_examples())
    {
        std::cout << "  " << name << ", " << ex.description << "\n";
    }
    std::cout << "\n";
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        print_usage();
        return 1;
    }

    std::string arg = argv[1];
    auto& examples = get_examples();

    if (examples.empty())
    {
        std::cerr << "No examples registered! Did you forget to link them?\n";
        return 1;
    }

    auto it = examples.find(arg);
    if (it == examples.end())
    {
        std::cerr << "Unknown example: " << arg << "\n\n";
        print_usage();
        return 1;
    }

    std::cout << "Running: " << it->second.description << "\n";
    return it->second.func();
}

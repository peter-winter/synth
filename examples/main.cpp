#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "register_example.h"

std::unordered_map<std::string, Example> examples;

std::string pretty_name(const std::string& func_name)
{
    if (func_name.ends_with("_main"))
    {
        std::string s = func_name.substr(0, func_name.size() - 5);  // remove "_main"
        std::string result;
        bool capitalize = true;
        for (char c : s)
        {
            if (c == '_')
            {
                result += ' ';
                capitalize = true;
            }
            else
            {
                result += capitalize ? std::toupper(c) : c;
                capitalize = false;
            }
        }
        return result;
    }
    return func_name;
}

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

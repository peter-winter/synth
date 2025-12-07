#include "register_example.h"
#include "factory.hpp"
#include <iostream>

int pink_noise()
{
    factory f(48000);

    auto synth = f.synthesizer(2, f.pink_noise());
    synth.set_master_level(0.85f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "Pink noise\n";
    std::cin.get();
    return 0;
}

REGISTER_EXAMPLE(pink_noise);

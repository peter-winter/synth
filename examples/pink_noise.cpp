#include "register_example.h"
#include "factory.hpp"
#include <iostream>

int pink_noise()
{
    factory<channels{2}, sample_rate{48000}> f;

    auto synth = f.synthesizer(f.pink_noise());
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

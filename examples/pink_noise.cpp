#include "register_example.h"
#include "factory.hpp"
#include <iostream>

int pink_noise()
{
    factory f(48000);

    timeline t;
    t.sound_on_at(0, 1);
    auto i = f.mono_instrument(std::move(t), f.pink_noise());
    
    auto synth = f.synthesizer(2, std::move(i));
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

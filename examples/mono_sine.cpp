#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"

#include <iostream>

int mono_sine()
{
    factory f(48000);

    auto mono_sine = f.sine_wave(constant(440_Hz));
    auto synth = f.synthesizer(1, mono_sine);
    synth.set_master_level(0.7f);
    
    if (!synth.start())
    {
        return 1;
    }

    std::cout << "440 Hz mono sine wave.\n";
    std::cin.get();
    
    return 0;
}

REGISTER_EXAMPLE(mono_sine);

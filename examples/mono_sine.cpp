#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"

#include <iostream>

int mono_sine()
{
    factory f(48000);

    auto mono_sine = f.sine_wave(constant(440_Hz));
    
    timeline t;
    t.sound_on_at(0, 1);
    auto i = f.single_voice_instrument(std::move(t), mono_sine);
    
    auto synth = f.synthesizer(1, std::move(i));
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

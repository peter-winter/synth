#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"

#include <iostream>

int pitch_vibrato()
{
    factory f(48000);

    auto lfo = f.sine_wave(constant(5_Hz));
    auto vibrato = f.mul_add(lfo, constant(5_Hz), constant(440_Hz));
    auto sweeping = f.sine_wave(vibrato);
    
    timeline t;
    auto i = f.instrument(t, sweeping);
    
    auto synth = f.synthesizer(2, i);
    synth.set_master_level(0.7f);

    if (!synth.start())
    {
        std::cerr << "Failed to start audio\n";
        return 1;
    }

    std::cout << "Pitch vibrato (-5..+5)Hz around 440Hz\n";
    std::cin.get();
    return 0;
}

REGISTER_EXAMPLE(pitch_vibrato);

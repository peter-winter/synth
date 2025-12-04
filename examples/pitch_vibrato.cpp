#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"

#include <iostream>

int pitch_vibrato()
{
    factory<channels{2}, sample_rate{48000}> f;

    auto lfo = f.sine_wave(constant(5_Hz));
    auto vibrato = f.mul_add(lfo, constant(5_Hz), constant(440_Hz));
    auto sweeping = f.sine_wave(vibrato);
    
    auto synth = f.synthesizer(sweeping);
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

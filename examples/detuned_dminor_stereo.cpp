#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"
#include "notes.hpp"

#include <iostream>

int detuned_dminor_stereo()
{
    factory f(48000);

    // Eight detuned oscillators — classic rich chord
    auto voices = f.mix(
        f.sine_wave(constant(notes::D(3))),
        f.sine_wave(constant(notes::D(3) + 0.3_Hz)),
        f.sine_wave(constant(notes::A(3))),
        f.sine_wave(constant(notes::A(3) + 0.4_Hz)),
        f.sine_wave(constant(notes::F(3))),
        f.sine_wave(constant(notes::F(3) - 0.25_Hz)),
        f.sine_wave(constant(notes::D(4))),
        f.sine_wave(constant(notes::D(4) - 0.15_Hz))
    );

    timeline t;
    auto i = f.instrument(t, voices);
    
    // Proper polyphony mixing with equal-amplitude (safe default)
    auto synth = f.synthesizer(2, i);
    synth.set_master_level(0.7f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "Rich detuned D minor chord\n";
    std::cin.get();
    
    return 0;
}

REGISTER_EXAMPLE(detuned_dminor_stereo);

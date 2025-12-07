#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"

#include <iostream>

int stereo_square_copier()
{
    factory f(48000);

    auto mono = f.square_wave(constant(220_Hz));

    timeline t;
    auto i = f.instrument(t, mono);
    
    auto synth = f.synthesizer(2, i);
    synth.set_master_level(0.7f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "220 Hz square wave (copied to both channels)\n";
    std::cin.get();
    
    return 0;
}

REGISTER_EXAMPLE(stereo_square_copier);

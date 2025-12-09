#include "register_example.h"
#include "factory.hpp"
#include <iostream>

int white_noise()
{
    factory f(48000);

    auto noise = f.white_noise();
    auto limited = f.peak_limiter(noise, constant(0.2f)); // tame the peaks

    timeline t;
    t.sound_on_at(0, 1);
    auto i = f.mono_instrument(std::move(t), limited);
    
    auto synth = f.synthesizer(2, std::move(i));
    synth.set_master_level(0.85f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "Pure white noise\n";
    std::cin.get();
    return 0;
}

REGISTER_EXAMPLE(white_noise);

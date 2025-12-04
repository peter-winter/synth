#include "register_example.h"
#include "factory.hpp"
#include <iostream>

int white_noise()
{
    factory<channels{2}, sample_rate{48000}> f;

    auto noise = f.white_noise();
    auto limited = f.peak_limiter(std::move(noise), constant(0.2f)); // tame the peaks

    auto synth = f.synthesizer(std::move(limited));
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

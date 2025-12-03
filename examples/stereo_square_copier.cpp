#include "factory.hpp"
#include "strong_types.hpp"
#include "frequency.hpp"

#include <iostream>

int main()
{
    factory<channels{2}, sample_rate{48000}> f;

    auto mono = f.square_wave(const_frequency(220_Hz));

    auto synth = f.synthesizer(std::move(mono));
    synth.set_master_level(0.7f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "220 Hz square wave (copied to both channels)\n";
    std::cin.get();
}

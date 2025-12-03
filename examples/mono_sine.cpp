#include "factory.hpp"
#include "strong_types.hpp"
#include "frequency.hpp"

#include <iostream>

int main()
{
    factory<channels{1}, sample_rate{48000}> f;

    auto mono_sine = f.sine_wave(const_frequency(440_Hz));
    auto synth = f.synthesizer(std::move(mono_sine));
    synth.set_master_level(0.7f);
    
    if (!synth.start())
    {
        return 1;
    }

    std::cout << "440 Hz mono sine wave.\n";
    std::cin.get();
}

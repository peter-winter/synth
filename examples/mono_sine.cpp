#include "factory.hpp"
#include "strong_types.hpp"
#include "frequency.hpp"

#include <iostream>

int main()
{
    factory<channels{1}, sample_rate{48000}> f;

    auto gen = f.sine_wave(const_frequency(440_Hz));  // A4
    auto mono = f.mono_adapter(std::move(gen));  // converts sample → array<sample,1>

    auto synth = f.synthesizer(std::move(mono), 0.5f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "440 Hz mono sine wave.\n";
    std::cin.get();
}

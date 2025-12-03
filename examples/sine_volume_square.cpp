// smooth_fade_square_stereo.cpp
#include "factory.hpp"
#include "frequency.hpp"
#include <iostream>

int main()
{
    factory<channels{2}, sample_rate{48000}> f;

    auto square = f.square_wave(const_frequency(220_Hz));

    // 0.25 Hz LFO → 4-second swell
    auto lfo = f.sine_wave(const_frequency(0.25_Hz));
    auto volume_env = f.mul_add(std::move(lfo), constant(0.5f), constant(0.5f)); // -1..1 → 0..1

    auto modulated = f.volume(std::move(square), std::move(volume_env));

    // One modulated mono signal → auto-copied to stereo
    auto synth = f.synthesizer(std::move(modulated));
    synth.set_master_level(0.7f);

    if (!synth.start())
    {
        std::cerr << "Failed to start audio\n";
        return 1;
    }

    std::cout << "220 Hz square wave with perfect 4-second volume swell\n";
    std::cout << "2 seconds fade-in → 2 seconds fade-out → repeat\n";
    std::cin.get();
}

// smooth_fade_square_stereo.cpp
#include "factory.hpp"
#include "frequency.hpp"
#include <iostream>

int main()
{
    constexpr sample_rate sr{48000};
    constexpr channels    ch{2};
    factory<ch, sr> f;

    auto square = f.square_wave(const_frequency(220_Hz));

    // 0.25 Hz = 4-second cycle -> 2 s fade in, 2 s fade out
    auto lfo = f.sine_wave(const_frequency(0.25_Hz));

    // Transform sine (-1..+1) -> 0..1 volume range
    auto volume_env = f.mul_add(
        std::move(lfo),
        constant(0.5f),   // mul -> -0.5..0.5
        constant(0.5f)    // add -> 0..1
    );

    auto modulated = f.volume(std::move(square), std::move(volume_env));
    auto stereo    = f.channel_copier(std::move(modulated));
    auto synth     = f.synthesizer(std::move(stereo), 0.5f);

    if (!synth.start())
    {
        std::cerr << "Failed to start audio\n";
        return 1;
    }

    std::cout << "220 Hz square wave with perfect 4-second volume swell\n";
    std::cout << "2 seconds fade-in → 2 seconds fade-out → repeat\n";
    std::cin.get();
}

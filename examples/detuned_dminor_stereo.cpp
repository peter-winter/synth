#include "factory.hpp"
#include "frequency.hpp"
#include <iostream>

int main()
{
    factory<channels{2}, sample_rate{48000}> f;

    constexpr frequency D3 = 146.83_Hz;
    constexpr frequency F3 = 174.61_Hz;
    constexpr frequency A3 = 220.00_Hz;
    constexpr frequency D4 = 293.66_Hz;

    auto left  = std::make_tuple(
        f.sine_wave(const_frequency(D3)),
        f.sine_wave(const_frequency(D3 + 0.3_Hz)),
        f.sine_wave(const_frequency(A3)),
        f.sine_wave(const_frequency(A3 + 0.4_Hz))
    );

    auto right = std::make_tuple(
        f.sine_wave(const_frequency(F3)),
        f.sine_wave(const_frequency(F3 - 0.25_Hz)),
        f.sine_wave(const_frequency(D4)),
        f.sine_wave(const_frequency(D4 - 0.15_Hz))
    );

    auto agg   = f.channel_aggregator(std::move(left), std::move(right));
    auto synth = f.synthesizer(std::move(agg), 0.5f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "Rich detuned D minor chord\n";
    std::cin.get();
}

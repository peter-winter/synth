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

    // Eight detuned oscillators — classic rich chord
    auto voices = f.mix(
        f.sine_wave(const_frequency(D3)),
        f.sine_wave(const_frequency(D3 + 0.3_Hz)),
        f.sine_wave(const_frequency(A3)),
        f.sine_wave(const_frequency(A3 + 0.4_Hz)),
        f.sine_wave(const_frequency(F3)),
        f.sine_wave(const_frequency(F3 - 0.25_Hz)),
        f.sine_wave(const_frequency(D4)),
        f.sine_wave(const_frequency(D4 - 0.15_Hz))
    );

    // Proper polyphony mixing with equal-amplitude (safe default)
    auto synth = f.synthesizer(std::move(voices));
    synth.set_master_level(0.7f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "Rich detuned D minor chord\n";
    std::cin.get();
}

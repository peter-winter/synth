#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"
#include "notes.hpp"

#include <iostream>

int supersaw_unison()
{
    factory<channels{2}, sample_rate{48000}> f;

    // 8 detuned saws → classic JP-8000 / Virus TI supersaw
    auto voices = f.unison(
        f.saw_wave(constant(notes::A(2) - 0.20_Hz)),
        f.saw_wave(constant(notes::A(2) - 0.12_Hz)),
        f.saw_wave(constant(notes::A(2) - 0.05_Hz)),
        f.saw_wave(constant(notes::A(2))),
        f.saw_wave(constant(notes::A(2) + 0.06_Hz)),
        f.saw_wave(constant(notes::A(2) + 0.13_Hz)),
        f.saw_wave(constant(notes::A(2) + 0.19_Hz)),
        f.saw_wave(constant(notes::A(2) + 0.27_Hz))
    );

    auto safe = f.peak_limiter(std::move(voices));

    auto synth = f.synthesizer(std::move(safe));
    synth.set_master_level(0.7f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "8-voice detuned supersaw (equal-power unison) — A2\n";
    std::cin.get();
    return 0;
}

REGISTER_EXAMPLE(supersaw_unison);

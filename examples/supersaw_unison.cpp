#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"
#include "notes.hpp"

#include <iostream>

int supersaw_unison()
{
    factory f(48000);

    // 8 detuned saws → classic JP-8000 / Virus TI supersaw
    auto voices = f.unison(
        f.down_saw_wave(constant(notes::A(2) - 0.20_Hz)),
        f.down_saw_wave(constant(notes::A(2) - 0.12_Hz)),
        f.down_saw_wave(constant(notes::A(2) - 0.05_Hz)),
        f.down_saw_wave(constant(notes::A(2))),
        f.down_saw_wave(constant(notes::A(2) + 0.06_Hz)),
        f.down_saw_wave(constant(notes::A(2) + 0.13_Hz)),
        f.down_saw_wave(constant(notes::A(2) + 0.19_Hz)),
        f.down_saw_wave(constant(notes::A(2) + 0.27_Hz))
    );

    auto safe = f.peak_limiter(voices);

    timeline t;
    t.sound_on_at(0, 1);
    auto i = f.mono_instrument(std::move(t), safe);
    
    auto synth = f.synthesizer(2, std::move(i));
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

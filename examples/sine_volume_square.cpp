#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"

#include <iostream>

int sine_volume_square()
{
    factory f(48000);

    auto square = f.square_wave(constant(220_Hz));

    // 0.25 Hz LFO -> 4-second swell
    auto lfo = f.sine_wave(constant(0.25_Hz));
    auto volume_env = f.bias(lfo); // -1..1 -> 0..1

    auto modulated = f.volume(square, volume_env);

    // One modulated mono signal -> auto-copied to stereo
    
    timeline t;
    t.sound_on_at(0, 1);
    auto i = f.single_voice_instrument(std::move(t), modulated);
    
    auto synth = f.synthesizer(2, std::move(i));
    synth.set_master_level(0.7f);

    if (!synth.start())
    {
        return 1;
    }

    std::cout << "220 Hz square wave with perfect 4-second volume swell\n";
    std::cout << "2 seconds fade-in → 2 seconds fade-out → repeat\n";
    std::cin.get();
    
    return 0;
}

REGISTER_EXAMPLE(sine_volume_square);

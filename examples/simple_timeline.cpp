#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"
#include "notes.hpp"

#include <iostream>

int simple_timeline()
{
    factory f(48000);

    auto mono_sine = f.volume(f.sine_wave(f.note_frequency()), f.switch_value());
    
    timeline t;
    t.note_on_at(f.sample_rate_ * 1, 1, notes::C(4));
    t.note_off_at(f.sample_rate_ * 3, 1);
    t.note_on_at(f.sample_rate_ * 4, 2, notes::E(4));
    t.note_off_at(f.sample_rate_ * 6, 2);
    
    auto i = f.single_voice_instrument(std::move(t), mono_sine);
    
    auto synth = f.synthesizer(2, std::move(i));
    synth.set_master_level(0.7f);
        
    if (!synth.start())
    {
        return 1;
    }

    std::cout << "440 Hz mono sine wave, several note sequence on and off.\n";
    std::cin.get();
    
    return 0;
}

REGISTER_EXAMPLE(simple_timeline);

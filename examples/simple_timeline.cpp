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
    auto noise = f.volume(f.pink_noise(), f.mul(f.switch_value(), constant(0.5f)));
    
    timeline t1;
    
    t1.note_on_at(f.sample_rate_ * 1, 1, notes::C(4));
    t1.note_off_at(f.sample_rate_ * 3, 1);
    t1.note_on_at(f.sample_rate_ * 4, 2, notes::E(4));
    t1.note_off_at(f.sample_rate_ * 6, 2);
    
    timeline t2;
    t2.sound_on_at(f.sample_rate_ * 1, 1);
    t2.sound_off_at(f.sample_rate_ * 3, 1);
    t2.sound_on_at(f.sample_rate_ * 4, 2);
    t2.sound_off_at(f.sample_rate_ * 6, 2);
    
    auto i1 = f.single_voice_instrument(std::move(t1), mono_sine);
    auto i2 = f.single_voice_instrument(std::move(t2), noise);
    
    auto synth = f.synthesizer(2, std::move(i1), std::move(i2));
    synth.set_master_level(0.7f);
        
    if (!synth.start())
    {
        return 1;
    }

    std::cout << "Sine wave (L) + noise (R), several notes sequence.\n";
    std::cin.get();
    
    return 0;
}

REGISTER_EXAMPLE(simple_timeline);

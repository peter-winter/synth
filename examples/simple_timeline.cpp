#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"
#include "notes.hpp"

#include <iostream>

int simple_timeline()
{
    using namespace std::chrono_literals;
    
    factory f(48000);

    auto wave = f.volume(f.triangle_wave(constant(300_Hz)), f.env_ar(constant(0.3s), constant(0.3s)));
    
    timeline t;
    t.sound_on_at(f.sample_rate_ * 1, 1);
    t.sound_off_at(f.sample_rate_ * 3, 1);
    t.sound_on_at(f.sample_rate_ * 4, 2);
    t.sound_off_at(f.sample_rate_ * 6, 2);
    
    auto i = f.mono_instrument(std::move(t), wave);
    
    auto synth = f.synthesizer(2, std::move(i));
    synth.set_master_level(0.7f);
        
    if (!synth.start())
    {
        return 1;
    }

    std::cout << "Sine wave (L) + (R), several notes sequence.\n";
    std::cin.get();
    
    return 0;
}

REGISTER_EXAMPLE(simple_timeline);

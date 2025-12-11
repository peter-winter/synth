#include "register_example.h"
#include "factory.hpp"
#include "frequency.hpp"
#include "constant.hpp"
#include "notes.hpp"

#include <iostream>

int poly_instrument_progression()
{
    using namespace std::chrono_literals;
    
    factory f(48000);

    auto saw = f.volume(f.saw_wave(f.note_frequency()), f.env_ar(constant(0.3s), constant(0.3s)));
    
    timeline t;
    
    t.note_on_at(f.sample_rate_ * 1, 1, notes::C(3));
    t.note_on_at(f.sample_rate_ * 2, 2, notes::E(3));
    t.note_on_at(f.sample_rate_ * 3, 3, notes::G(3));
    t.note_on_at(f.sample_rate_ * 4, 4, notes::C(4));
    t.note_on_at(f.sample_rate_ * 5, 5, notes::E(4));
    t.note_on_at(f.sample_rate_ * 6, 6, notes::G(4));
    t.note_on_at(f.sample_rate_ * 7, 7, notes::C(5));
    t.note_on_at(f.sample_rate_ * 8, 8, notes::E(5));
    
    t.note_off_at(f.sample_rate_ * 10, 1);
    t.note_off_at(f.sample_rate_ * 10, 2);
    t.note_off_at(f.sample_rate_ * 10, 3);
    t.note_off_at(f.sample_rate_ * 10, 4);
    t.note_off_at(f.sample_rate_ * 10, 5);
    t.note_off_at(f.sample_rate_ * 10, 6);
    t.note_off_at(f.sample_rate_ * 10, 7);
    t.note_off_at(f.sample_rate_ * 10, 8);
    
    auto ins = f.unison_instrument(3, std::move(t), saw);
    
    auto synth = f.synthesizer(2, std::move(ins));
    synth.set_master_level(0.5f);
        
    if (!synth.start())
    {
        return 1;
    }

    std::cout << "Poly instrument, simple note progression, stealing, only 3 playing.\n";
    std::cin.get();

    return 0;
}

REGISTER_EXAMPLE(poly_instrument_progression);

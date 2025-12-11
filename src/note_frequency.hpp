#pragma once

#include "voice.hpp"
#include "frequency.hpp"
#include "utility.hpp"

constexpr frequency off_frequency = 0.000001_Hz;

class note_frequency
{
public:
    explicit note_frequency(voice* v)
        : v_(v)
    {}

    frequency operator()()
    {
        return v_->f_.value_or(off_frequency);
    }

private:
    voice* v_{nullptr};
};

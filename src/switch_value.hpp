#pragma once

#include "voice.hpp"
#include "utility.hpp"

class switch_value
{
public:
    explicit switch_value(voice* v)
        : v_(v)
    {}
    
    float operator()()
    {
        return (v_ != nullptr && v_->active_) ? 1.0f : 0.0f;
    }
    
private:
    voice* v_{nullptr};
};

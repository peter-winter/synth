#pragma once

#include <cstddef>
#include <cstdint>

struct sample_rate
{
    constexpr explicit sample_rate(uint32_t value):
        value_(value)
    {}
    
    uint32_t value_;
};

struct channels
{
    constexpr explicit channels(size_t value):
        value_(value)
    {}
    
    size_t value_;
};

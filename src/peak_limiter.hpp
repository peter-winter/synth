#pragma once

#include "constant.hpp"

#include <cmath>
#include <type_traits>
#include <utility>

template <typename InputGen, typename CeilingGen, typename ReleaseGen>
class peak_limiter
{
public:
    explicit peak_limiter(InputGen&& in, CeilingGen&& ceiling = constant(0.95f), ReleaseGen&& release = constant(0.999f))
        : input_(std::forward<InputGen>(in))
        , ceiling_gen_(std::forward<CeilingGen>(ceiling))
        , release_gen_(std::forward<ReleaseGen>(release))
        , peak_(0.0f)
    {}

    float operator()()
    {
        float in      = input_();
        float ceiling = ceiling_gen_();
        float release = release_gen_();

        // One-pole peak detector (leaky max)
        float abs_sample = std::abs(in);
        peak_ = std::max(abs_sample, peak_ * release);

        // Soft gain reduction only when needed
        float gain = (peak_ > ceiling) ? (ceiling / peak_) : 1.0f;
        return in * gain;
    }

private:
    InputGen input_;
    CeilingGen ceiling_gen_;
    ReleaseGen release_gen_;
    float peak_;
};

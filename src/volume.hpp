#pragma once

#include "math.hpp"

#include <cmath>

template <typename G, typename V>
class volume
{
public:
    explicit volume(G g, V v)
        : sample_gen_(g), volume_gen_(v)
    {
    }

    float operator()()
    {
        float sample = sample_gen_();
        float vol  = volume_gen_();           // still 0.0–1.0 from user
        float gain = math::perceptual_gain(vol);    // logarithmic magic
        return sample * gain;
    }

private:
    G sample_gen_;
    V volume_gen_;
};

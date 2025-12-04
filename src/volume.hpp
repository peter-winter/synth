#pragma once

#include "math.hpp"

#include <cmath>

template <typename SampleGenerator, typename VolumeGenerator>
class volume
{
public:
    explicit volume(SampleGenerator&& samp_gen,
                    VolumeGenerator&& vol_gen)
        : sample_gen_(std::forward<SampleGenerator>(samp_gen))
        , volume_gen_(std::forward<VolumeGenerator>(vol_gen))
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
    SampleGenerator sample_gen_;
    VolumeGenerator volume_gen_;
};

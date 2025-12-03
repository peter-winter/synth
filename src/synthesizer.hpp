// synthesizer.hpp
#pragma once

#include "strong_types.hpp"
#include "math.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <atomic>
#include <iostream>


template <channels Ch, sample_rate Sr, typename Generator>
class synthesizer
{
public:
    explicit synthesizer(Generator&& gen, float master_level)
        : generator_(std::move(gen))
        , master_level_(master_level)
    {
        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        config.playback.format   = ma_format_f32;
        config.playback.channels = static_cast<ma_uint32>(Ch.value_);
        config.sampleRate        = Sr.value_;
        config.dataCallback      = data_callback;
        config.pUserData         = this;

        if (ma_device_init(nullptr, &config, &device_) != MA_SUCCESS)
        {
            std::cerr << "Failed to initialize audio device.\n";
            return;
        }

        is_initialized_ = true;
    }

    ~synthesizer()
    {
        stop();
        if (is_initialized_)
            ma_device_uninit(&device_);
    }

    bool start()
    {
        if (!is_initialized_ || running_)
            return false;

        if (ma_device_start(&device_) != MA_SUCCESS)
        {
            std::cerr << "Failed to initialize audio device.\n";
            return false;
        }

        running_ = true;
        return true;
    }

    void stop()
    {
        if (running_)
        {
            ma_device_stop(&device_);
            running_ = false;
        }
    }

    bool is_running() const { return running_; }

private:
    static void data_callback(ma_device* device, void* output, const void*, ma_uint32 frame_count)
    {
        auto* self = static_cast<synthesizer*>(device->pUserData);
        if (!self || !self->running_)
            return;

        float* out    = static_cast<float*>(output);
        float  level  = self->master_level_;

        for (ma_uint32 f = 0; f < frame_count; ++f)
        {
            std::array<float, Ch.value_> frame = self->generator_();

            for (size_t ch = 0; ch < Ch.value_; ++ch)
                out[f * Ch.value_ + ch] = frame[ch] * perceptual_gain(level);
        }
    }

    ma_device         device_{};
    Generator         generator_;
    float             master_level_ = 0.25f;

    std::atomic<bool> running_{false};
    bool              is_initialized_{false};
};

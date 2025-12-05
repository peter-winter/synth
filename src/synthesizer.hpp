// synthesizer.hpp
#pragma once

#include "strong_types.hpp"
#include "math.hpp"

#include <miniaudio.h>

#include <iostream>

template <channels Ch, sample_rate Sr, typename... Gs>
    requires (sizeof...(Gs) > 0 && sizeof...(Gs) <= Ch.value_)
class synthesizer
{
    static constexpr size_t voice_count = sizeof...(Gs);

public:
    explicit synthesizer(Gs... g)
        : generators_(g...)
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
    
    void set_master_level(float master_level = 0.5f) { master_level_ = master_level; }

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
            std::cerr << "Failed to start audio device.\n";
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
        if (!self || !self->running_) return;

        float* out = static_cast<float*>(output);
        float level = self->master_level_;

        for (ma_uint32 f = 0; f < frame_count; ++f)
        {
            // Generate all available mono signals
            std::array<float, voice_count> voices = std::apply(
                [](auto&... gen) { return std::array{gen()...}; },
                self->generators_
            );

            // Distribute to channels: repeat/fill pattern
            for (size_t ch = 0; ch < Ch.value_; ++ch)
            {
                float sample = voices[ch % voice_count];
                out[f * Ch.value_ + ch] = sample * math::perceptual_gain(level);
            }
        }
    }

    ma_device device_{};
    std::tuple<Gs...> generators_;
    float master_level_ = 0.5f;

    bool running_{false};
    bool is_initialized_{false};
};

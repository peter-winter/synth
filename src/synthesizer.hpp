// synthesizer.hpp
#pragma once

#include "math.hpp"

#include <miniaudio.h>

#include <iostream>

template <typename... Is>
    requires (sizeof...(Is) > 0)
class synthesizer
{
    static constexpr size_t voice_count = sizeof...(Is);

public:
    explicit synthesizer(uint32_t sample_rate, uint32_t channel_count, Is... is)
        : instruments_(std::move(is)...), sample_rate_(sample_rate), channel_count_(channel_count)
    {
        if ((channel_count_ % voice_count) != 0 || voice_count > channel_count_)
            throw std::runtime_error("Invalid number of channels / voice generators");
            
        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        config.playback.format   = ma_format_f32;
        config.playback.channels = channel_count_;
        config.sampleRate        = sample_rate_;
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

        running_ = true;
        
        std::apply(
            [](auto&... is) { (is.reset(), ...); },
            instruments_);
            
        if (ma_device_start(&device_) != MA_SUCCESS)
        {
            std::cerr << "Failed to start audio device.\n";
            return false;
        }
        
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
                [](auto&... is) { return std::array{is()...}; },
                self->instruments_
            );

            uint32_t channel_count = self->channel_count_;
            // Distribute to channels
            for (size_t ch = 0; ch < channel_count; ++ch)
            {
                float sample = voices[ch % voice_count];
                out[f * channel_count + ch] = sample * math::perceptual_gain(level);
            }
        }
    }

    ma_device device_{};
    std::tuple<Is...> instruments_;
    float master_level_{0.5f};

    bool running_{false};
    bool is_initialized_{false};
    uint32_t sample_rate_;
    uint32_t channel_count_;
};

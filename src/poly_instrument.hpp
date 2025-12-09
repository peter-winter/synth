#pragma once

#include "timeline.hpp"
#include "voice.hpp"
#include "mix.hpp"
#include "utility.hpp"

#include <vector>

template<typename PatchBuilder>
class poly_instrument
{
    struct voice_slot
    {
        voice v{};
        std::optional<to_g_t<PatchBuilder>> patch;
        float level_env{0.0f};
    };

    static constexpr size_t invalid = size_t(-1);

public:
    poly_instrument(uint32_t max_voices, timeline t, PatchBuilder patch_b)
        : max_voices_(max_voices),
          t_(std::move(t)),
          patch_b_(patch_b),
          voices_(max_voices_),
          order_(max_voices_),
          note_id_to_voice_(max_voices_, invalid)
    {}
    
    poly_instrument(poly_instrument&& other) = default;
    
    poly_instrument(const poly_instrument&) = delete;
    poly_instrument& operator = (const poly_instrument&) = delete;

    float operator()()
    {
        t_.prepare_events();
        process_events();

        float mixed = 0.0f;
        size_t audible_count = 0;
        size_t write_pos = 0;

        for (size_t i = 0; i < active_count_; ++i)
        {
            size_t idx = order_[i];
            voice_slot& slot = voices_[idx];

            float sample = (*slot.patch)();

            float abs_s = std::abs(sample);
            slot.level_env = std::max(abs_s, slot.level_env * 0.9998f);

            mixed += sample;
            if (slot.level_env > 0.0005f) ++audible_count;

            if (slot.v.active_ || slot.level_env >= 0.0001f)
            {
                order_[write_pos++] = idx;
            }
            else
            {
                note_id_to_voice_[slot.v.note_id_ % max_voices_] = invalid;
                slot.patch.reset();
            }
        }

        active_count_ = write_pos;
        t_.inc();

        return mixed * polyphony_gain(audible_count, polyphony_scale::equal_power);
    }

    void reset()
    {
        t_.reset();
        active_count_ = 0;
        note_id_to_voice_.assign(max_voices_, invalid);
        voices_.assign(max_voices_, voice_slot{});
    }

private:
    void process_events()
    {
        for (const auto& ev : t_.get_events())
        {
            std::visit(overloaded{
                [this](const note_on&  on)  { do_on(on.id_, on.f_); },
                [this](const note_off& off) { do_off(off.id_); },
                [this](const sound_on& on)  { do_on(on.id_, std::nullopt); },
                [this](const sound_off& off){ do_off(off.id_); }
            }, ev.payload);
        }
    }

    void do_on(note_id id, std::optional<frequency> freq)
    {
        size_t idx = allocate_voice();
        note_id_to_voice_[id % max_voices_] = idx;

        auto& s = voices_[idx];
        s.v.active_ = true;
        s.v.note_id_ = id;
        s.v.f_ = freq;
        s.level_env = 1.0f;
        s.patch.emplace(patch_b_(&s.v));
    }

    void do_off(note_id id)
    {
        size_t idx = note_id_to_voice_[id % max_voices_];
        if (idx != invalid)
            voices_[idx].v.active_ = false;
    }

    size_t allocate_voice()
    {
        if (active_count_ < max_voices_)
            return order_[active_count_++] = active_count_;
        else
            return order_[0];  // steal oldest
    }

    const uint32_t max_voices_;
    timeline t_;
    PatchBuilder patch_b_;

    std::vector<voice_slot> voices_;
    std::vector<size_t> order_;
    size_t active_count_ = 0;

    std::vector<size_t> note_id_to_voice_;
};

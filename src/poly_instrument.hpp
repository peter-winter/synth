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

    static constexpr size_t sentinel = size_t(-1);
    
public:
    poly_instrument(uint32_t max_voices, timeline t, PatchBuilder patch_b)
        : max_voices_(max_voices),
        t_(std::move(t)),
        patch_b_(patch_b),
        voices_(max_voices_)
    {
        order_[0].resize(max_voices_, sentinel);
        order_[1].resize(max_voices_, sentinel);
    }

    float operator()()
    {
        t_.prepare_events();
        process_events();

        float mixed = 0.0f;
        size_t write_pos = 0;
        
        const auto& read_order = order_[read_idx_];
        auto& write_order = order_[write_idx_];

        for (size_t i = 0; i < active_count_; ++i)
        {
            size_t ring_idx = (head_ + i) % max_voices_;
            size_t slot_idx = read_order[ring_idx];
            voice_slot& slot = voices_[slot_idx];
            
            float sample = (*slot.patch)();

            float abs_s = std::abs(sample);
            slot.level_env = std::max(abs_s, slot.level_env * 0.9998f);

            mixed += sample;
            
            if (slot.v.active_ || slot.level_env >= 0.0001f)
            {
                write_order[write_pos++] = slot_idx;
            }
            else
            {
                slot.patch.reset();
            }
        }
    
        std::swap(read_idx_, write_idx_);
        head_ = 0;
        tail_ = write_pos;
        active_count_ = write_pos;
        t_.inc();

        return mixed * polyphony_gain(active_count_, polyphony_scale::equal_amplitude);
    }

    void reset()
    {
        t_.reset();
        head_ = tail_ = active_count_ = 0;
        voices_.assign(max_voices_, {});
        read_idx_ = 0;
        write_idx_ = 1;
        order_[0].assign(max_voices_, sentinel);
        order_[1].assign(max_voices_, sentinel);
    }
    
    size_t max_voices() const { return max_voices_; }

private:
    void process_events()
    {
        for (const auto& ev : t_.get_events())
        {
            std::visit(overloaded{
                [this](const note_on& on)  { do_on(on.id_, on.f_); },
                [this](const note_off& off) { do_off(off.id_); },
                [this](const sound_on& on)  { do_on(on.id_, std::nullopt); },
                [this](const sound_off& off){ do_off(off.id_); }
            }, ev.payload);
        }
    }

    void do_on(note_id id, std::optional<frequency> freq)
    {
        size_t idx = allocate_voice();

        auto& slot = voices_[idx];
        slot.v.active_ = true;
        slot.v.note_id_ = id;
        slot.v.f_ = freq;
        slot.level_env = 1.0f;
        slot.patch.emplace(patch_b_(&slot.v));
    }

    void do_off(note_id id)
    {
        auto& order = order_[read_idx_];
        
        for (size_t i = 0; i < active_count_; ++i)
        {
            size_t ring_idx = (head_ + i) % max_voices_;
            size_t idx = order[ring_idx];
            if (voices_[idx].v.note_id_ == id)
            {
                voices_[idx].v.active_ = false;
                return;
            }
        }
    }

    size_t allocate_voice()
    {
        auto& order = order_[read_idx_];
        if (active_count_ < max_voices_)
        {
            size_t idx = tail_;
            tail_ = (tail_ + 1) % max_voices_;
            order[idx] = idx;
            ++active_count_;
            return idx;
        }
        else
        {
            size_t stolen = order[head_];
            head_ = (head_ + 1) % max_voices_;
            return stolen;
        }
    }

    const uint32_t max_voices_;

    timeline t_;
    PatchBuilder patch_b_;

    std::vector<voice_slot> voices_;
    std::vector<size_t> order_[2];
    
    size_t head_{0};
    size_t tail_{0};
    size_t active_count_{0};
    
    size_t read_idx_{0};
    size_t write_idx_{1};
};

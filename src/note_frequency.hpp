#pragma once

#include "timeline.hpp"
#include "frequency.hpp"
#include "utility.hpp"

constexpr frequency off_frequency = 0.0001_Hz;

class note_frequency
{
public:
    explicit note_frequency(timeline* t)
        : t_(t), current_(off_frequency)
    {}

    frequency operator()()
    {
        auto events = t_->get_events();
        for (const auto& ev : events)
        {
            std::visit(overloaded
            {
                [this](const note_on& on)
                {
                    latched_id_ = on.id;
                    current_    = on.freq;
                    active_     = true;
                },
                [this](const note_off& off)
                {
                    if (active_ && off.id == latched_id_)
                        active_ = false;
                }
            }, ev.payload);
        }

        return active_ ? current_ : off_frequency;
    }

private:
    timeline* t_ = nullptr;
    note_id latched_id_ = 0;
    
    frequency current_;
    bool active_ = false;
};

#pragma once

#include "frequency.hpp"

#include <cstdint>
#include <variant>
#include <vector>
#include <ranges>
#include <algorithm>

using sample_index = uint64_t;
using note_id      = int32_t;

struct note_on
{
    note_id id_; 
    frequency f_;
};

struct note_off
{
    note_id id_;
};

struct sound_on
{
    note_id id_;
};

struct sound_off
{
    note_id id_;
};

using event = std::variant<note_on, note_off, sound_on, sound_off>;

class timeline
{
public:
    timeline() = default;
    timeline(timeline&&) = default;
    timeline(const timeline&) = delete;
    timeline& operator = (const timeline&) = delete;
    
    struct timed_event
    {
        sample_index when;
        event        payload;
    };

    void note_on_at(sample_index when, note_id id, frequency freq)
    {
        insert(timed_event{when, note_on{id, freq}});
    }

    void note_off_at(sample_index when, note_id id)
    {
        insert(timed_event{when, note_off{id}});
    }
    
    void sound_on_at(sample_index when, note_id id)
    {
        insert(timed_event{when, sound_on{id}});
    }
    
    void sound_off_at(sample_index when, note_id id)
    {
        insert(timed_event{when, sound_off{id}});
    }

    using events_view = std::ranges::subrange<std::vector<timed_event>::const_iterator>;
        
    void inc()
    {
        ++current_sample_;
    }
    
    void prepare_events()
    {
        if (next_event_it_ == events_.end() || current_sample_ < next_event_it_->when)
        {
            current_events_ = events_view{events_.end(), events_.end()};
            return;
        }
                
        auto upper = std::ranges::upper_bound(
            next_event_it_, events_.end(), current_sample_,
            std::less<>{}, &timed_event::when);

        current_events_ = events_view{next_event_it_, upper};
        next_event_it_ = upper;
    }
    
    events_view get_events() const
    {
        return current_events_;
    }

    void reset()
    {
        current_sample_ = 0;
        next_event_it_ = events_.begin();
    }
    
    sample_index current_sample() const { return current_sample_; }
    
private:
    void insert(const timed_event& te)
    {
        auto it = std::ranges::upper_bound(events_, te.when, std::less<>{}, &timed_event::when);
        events_.insert(it, te);
    }
    
    std::vector<timed_event> events_;
    sample_index current_sample_{0};
    std::vector<timed_event>::const_iterator next_event_it_{events_.begin()};
    events_view current_events_;
};

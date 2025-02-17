//! TODO: sort the includes
//! TODO: ake sure there are  no unused includes
//! TODO: consider making small functions here inline

#include <cstdint>
#include <utility>
#include <vector>
#include <memory>


class IEventProcessor
{
public:
using Integer = int64_t;

//////////////////////////////////////////////////////////////////////////
/// ---
struct ReservedEvent
{
    ReservedEvent();
    ReservedEvent(const Integer sequence_number, void *const event);

    ReservedEvent(const ReservedEvent&) = delete;
    ReservedEvent& operator=(const ReservedEvent&) = delete;

    ReservedEvent(ReservedEvent&&) = delete;
    ReservedEvent& operator=(ReservedEvent&&) = delete;

    Integer GetSequenceNumber() const { return sequence_number_; }
    void* GetEvent() const { return event_; }

    private:
    Integer sequence_number_;
    void* event_;
};

//////////////////////////////////////////////////////////////////////////
/// ---
struct ReservedEvents
{
    ReservedEvents(const Integer sequence_number,
    void *const event,
    const size_t count,
    const size_t event_size);

    ReservedEvents(const ReservedEvents&) = delete;
    ReservedEvents& operator=(const ReservedEvents&) = delete;

    ReservedEvents(ReservedEvents&&) = delete;
    ReservedEvents& operator=(ReservedEvents&&) = delete;

    template <class TEvent, class ...Args>
    void Emplace(const size_t index, Args&&... args)
    {
        auto event = static_cast<TEvent*>(GetEvent(index));
        if (event)
            std::construct_at(event, std::forward<Args>(args)...);
    }
        
    Integer GetSequenceNumber() const { return sequence_number_; }
    void* GetEvent(const size_t index) const;

    size_t Count() const { return count_; }

    private:
    Integer sequence_number_;
    void* events_;
    size_t count_;
    size_t event_size_;
};

//! TODO: consider imlpementing the template functions inside the header file
template <typename T>
std::pair<size_t, void* const> ReserveEvent();

//////////////////////////////////////////////////////////////////////////
/// ---
template <class T, class ...Args>
ReservedEvent Reserve(Args&&... args)
{
    const auto reservation = ReserveEvent<T>();
    if (!reservation.second)
    return ReservedEvent();

    //! TODO: for optimization
    std::construct_at(reservation.second, std::forward<Args>(args)...);
    return ReservedEvent(reservation.first, reservation.second);
}

//////////////////////////////////////////////////////////////////////////
/// ---
std::vector<ReservedEvents> ReserveRange(const size_t size); // TODO: static vector?

//////////////////////////////////////////////////////////////////////////
/// ---
void Commit(const Integer sequence_number);

//////////////////////////////////////////////////////////////////////////
/// ---
void Commit(const Integer sequence_number, const size_t count);
};

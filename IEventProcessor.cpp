#include "IEventProcessor.h"

//! multiple threads can push their events to the queue

//! TODO: Implement
IEventProcessor::ReservedEvent::ReservedEvent ()
{

}

//! TODO: implement
IEventProcessor::ReservedEvent::ReservedEvent(const Integer sequence_number, void *const event)
{

}

//! TODO: implement
IEventProcessor::ReservedEvents::ReservedEvents(const Integer sequence_number,
    void *const event,
    const size_t count,
    const size_t event_size)
{

}

//! TODO: implement
void* IEventProcessor::ReservedEvents::GetEvent(const size_t index) const
{

}

//! TODO: Implement
template <typename T>
std::pair<size_t, void* const> IEventProcessor::ReserveEvent() {
    return {0, nullptr};
}

//! TODO: Implement
std::vector<IEventProcessor::ReservedEvents> IEventProcessor::ReserveRange (const size_t size)
{
    
}

//! TODO: Implement
void  IEventProcessor::Commit (const Integer sequence_number) {

}

//! TODO: implement 
void IEventProcessor::Commit(const Integer sequence_number, const size_t count)
{

}

//! TODO: understand what the header files mean, make sure nothing is missed as a hint or concept
//! TODO: check instructions again
//! TODO: check the example
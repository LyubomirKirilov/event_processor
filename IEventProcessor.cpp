#include "IEventProcessor.h"

//! multiple threads can push their events to the queue

//! TODO: understand what the header files mean, make sure nothing is missed as a hint or concept
//! TODO: check instructions again
//! TODO: check the example
//! TODO: consider making this in a separate file
LockFreeQueue::LockFreeQueue ( ) {

}


IEventProcessor::IEventProcessor() {

}

//! TODO: Implement
IEventProcessor::ReservedEvent::ReservedEvent ()
{

}

//! TODO: implement
IEventProcessor::ReservedEvent::ReservedEvent(const Integer sequence_number, void *const event)
{

}

//! TODO: implement
bool IEventProcessor::ReservedEvent::IsValid() const
{
    return false;
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
    void* ptr = nullptr;
    return ptr;

}

//! TODO: implement
bool IEventProcessor::ReservedEvents::IsValid() const
{
    return false;
}

//! TODO: Implement
std::vector<IEventProcessor::ReservedEvents> IEventProcessor::ReserveRange (const size_t size)
{
    std::vector<IEventProcessor::ReservedEvents> reservedRange;

    return reservedRange;
}

//! TODO: Implement
void  IEventProcessor::Commit (const Integer sequence_number) {

}

//! TODO: implement 
void IEventProcessor::Commit(const Integer sequence_number, const size_t count)
{

}
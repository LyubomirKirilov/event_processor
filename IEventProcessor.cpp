#include <iostream>
#include <memory>
#include <thread>

#include "IEventProcessor.h"

IEventProcessor::IEventProcessor() 
    : stop_(false) 
{
    // Start the worker thread for processing events
    worker_thread_ = std::thread([this] { ProcessEvents(); });
}

//! should be ok
IEventProcessor::~IEventProcessor() {
    stop_ = true;
    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
}

//! should be ok
IEventProcessor::ReservedEvent::ReservedEvent()
    : sequence_number_(-1), event_(nullptr) 
{
}

//! should be ok
IEventProcessor::ReservedEvent::ReservedEvent(const Integer sequence_number, void* const event)
    : sequence_number_(sequence_number), event_(event) 
{
}

//! TODO: should not have a move here
IEventProcessor::ReservedEvents::ReservedEvents(ReservedEvents&& other) noexcept
    : sequence_number_(other.sequence_number_),
      events_(other.events_),
      count_(other.count_),
      event_size_(other.event_size_)
{
    // Nullify the source object to leave it in a valid state after the move
    other.events_ = nullptr;
    other.count_ = 0;
    other.event_size_ = 0;
}

//! should be ok
bool IEventProcessor::ReservedEvent::IsValid() const 
{
    return event_ != nullptr;
}

//! should be ok
IEventProcessor::ReservedEvents::ReservedEvents(const Integer sequence_number, 
    void* const event, 
    const size_t count, 
    const size_t event_size)
    : sequence_number_(sequence_number), events_(event), count_(count), event_size_(event_size) 
{
}

//! should be ok
void* IEventProcessor::ReservedEvents::GetEvent(const size_t index) const 
{
    if (index >= count_) 
        return nullptr;  // Prevent out-of-bounds access

    return static_cast<void*>(static_cast<char*>(events_) + index * event_size_);
}

//! should be ok
bool IEventProcessor::ReservedEvents::IsValid() const 
{
    return events_ != nullptr;
}


//! should be ok
void IEventProcessor::Commit(const Integer sequence_number) 
{
    queue_.Commit(sequence_number);
}

//! should be ok
void IEventProcessor::Commit(const Integer sequence_number, const size_t count) 
{
    for (size_t i = 0; i < count; ++i) {
        queue_.Commit(sequence_number + i);
    }
}

void IEventProcessor::ProcessEvents() {
    while (!stop_) {
        // Process and remove events from the queue
        while (auto event = queue_.Pop()) {
            // Process the event; the logic will depend on your use case
            // If events are of various types, you can use std::visit or other strategies to handle them
            std::cout << "Processing event..." << std::endl;
        }

        // Yield to avoid blocking CPU when queue is empty
        std::this_thread::yield();
    }
}


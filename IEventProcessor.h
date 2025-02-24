#pragma once

#include <atomic>
#include <ranges>
#include <algorithm>
#include <vector>
#include <thread>
#include <cstdint>
#include <utility>
#include <memory>
#include <iostream>
#include <optional>

#include "Event.h"
#include "LockFreeEventQueue.h"

class IEventProcessor
{
public:
    using Integer = int64_t;

    IEventProcessor();
    ~IEventProcessor();

    /**
     * @brief Struct representing a single reserved event.
     * 
     * This struct holds the event data that has been reserved in memory.
     */
    struct ReservedEvent
    {
        ReservedEvent();
        
        /**
         * @brief Constructor for ReservedEvent.
         * @param sequence_number The sequence number assigned to the event.
         * @param event Pointer to the allocated event.
         */
        ReservedEvent(const Integer sequence_number, void* const event);

        ReservedEvent(const ReservedEvent&) = delete;
        ReservedEvent& operator=(const ReservedEvent&) = delete;

        ReservedEvent(ReservedEvent&&) = delete;
        ReservedEvent& operator=(ReservedEvent&&) = delete;

        /**
         * @brief Retrieves the sequence number of the event.
         * @return The sequence number assigned to the event.
         */
        Integer GetSequenceNumber() const { return sequence_number_; }

        /**
         * @brief Retrieves the pointer to the event data.
         * @return Pointer to the event.
         */
        void* GetEvent() const { return event_; }

        /**
         * @brief Checks if the ReservedEvent is valid.
         * @return True if the event is valid, false otherwise.
         */
        bool IsValid() const;

    private:
        Integer sequence_number_;
        void* event_;
    };

    /**
     * @brief Struct representing a batch of reserved events.
     * 
     * This struct holds a batch of events that have been reserved in memory.
     */
    struct ReservedEvents
    {
        /**
         * @brief Constructor for ReservedEvents.
         * @param sequence_number The sequence number for the first event in the batch.
         * @param event Pointer to the allocated event batch.
         * @param count The number of events in the batch.
         * @param event_size The size of a single event.
         */
        ReservedEvents(const Integer sequence_number, void* const event, const size_t count, const size_t event_size);

        ReservedEvents(const ReservedEvents&) = delete;
        ReservedEvents& operator=(const ReservedEvents&) = delete;

        ReservedEvents(ReservedEvents&&) noexcept; //! TODO: this to be made delete
        ReservedEvents& operator=(ReservedEvents&&) = delete;

        /**
         * @brief Constructs an event in the reserved batch at the specified index.
         * @param index The index of the event to be constructed.
         * @param args Arguments to be passed to the event constructor.
         */
        template <class TEvent, class... Args>
        void Emplace(const size_t index, Args&&... args); //! TODO: to be implemented, check if it is in the example

        /**
         * @brief Retrieves the sequence number of the first event in the batch.
         * @return The sequence number assigned to the first event.
         */
        Integer GetSequenceNumber() const { return sequence_number_; }

        /**
         * @brief Retrieves a pointer to a specific event in the batch.
         * @param index The index of the event within the batch.
         * @return Pointer to the requested event.
         */
        void* GetEvent(const size_t index) const;

        /**
         * @brief Retrieves the number of events in the batch.
         * @return The number of events in the batch.
         */
        size_t Count() const { return count_; }

        /**
         * @brief Checks if the ReservedEvents batch is valid.
         * @return True if the batch is valid, false otherwise.
         */
        bool IsValid() const;

    private:
        Integer sequence_number_;
        void* events_;
        size_t count_;
        size_t event_size_;
    };

public:
    /**
     * @brief Reserves memory for a single event of type T and returns a pointer to it.
     * 
     * This method reserves memory in the event queue for a single event of the 
     * specified type and returns the sequence number along with the allocated 
     * event pointer.
     * 
     * @tparam T The type of the event (e.g., EventA, EventB, etc.).
     * @return A pair consisting of the sequence number and a pointer to the reserved event.
     */
    template <typename T, class... Args>
    std::pair<size_t, void*> ReserveEvent(Args&&... args);

    /**
     * @brief Reserves memory for a single event, constructs it, and returns a ReservedEvent.
     * 
     * This method reserves memory in the event queue for a single event, 
     * constructs the event using the provided arguments, and returns a 
     * ReservedEvent object containing the reserved event data.
     * 
     * @tparam T The type of the event (e.g., EventA, EventB, etc.).
     * @param args Arguments for constructing the event.
     * @return A ReservedEvent object containing the reserved event data.
     */
    template <class T, class... Args>
    ReservedEvent Reserve(Args&&... args);

    /**
     * @brief Reserves memory for a range of events and returns a collection of ReservedEvents.
     * 
     * This method reserves memory for a specified number of events in the queue 
     * and returns a collection of ReservedEvents representing the reserved memory.
     * 
     * @param size The number of events to reserve.
     * @return A vector of ReservedEvents objects containing the reserved events.
     */
    std::vector<ReservedEvents> ReserveRange(size_t count);

    /**
     * @brief Commits a single reserved event to the event processor.
     * 
     * This method commits the event identified by the provided sequence number
     * to the event processor, making it available for processing.
     * 
     * @param sequence_number The sequence number of the event to commit.
     */
    void Commit(const Integer sequence_number);

    /**
     * @brief Commits a batch of reserved events to the event processor.
     * 
     * This method commits the events identified by the provided sequence number
     * and count to the event processor, making them available for processing.
     * 
     * @param sequence_number The sequence number of the first event in the batch.
     * @param count The number of events to commit.
     */
    void Commit(const Integer sequence_number, const size_t count);

    /**
     * @brief Processes events in the event queue.
     * 
     * This method runs a loop to process events continuously, popping events 
     * from the queue and performing necessary processing.
     */
    void ProcessEvents();

    /**
     * @brief Publishes a single event to the event processor.
     * 
     * This method reserves memory for an event of type T, constructs it using 
     * the provided arguments, and then commits the event to the processor.
     * 
     * @tparam TEvent The type of the event (e.g., EventA, EventB, etc.).
     * @param args Arguments to construct the event.
     */
public:
    template <typename TEvent, typename... Args>
    void PublishSingleEvents(IEventProcessor& processor, size_t count, Args&&... args);

    /**
     * @brief Publishes multiple events to the event processor.
     * 
     * This method reserves space for multiple events of type T, constructs them
     * with the provided arguments, and commits them to the processor.
     * 
     * @tparam TEvent The type of the event (e.g., EventA, EventB, etc.).
     * @param count The number of events to reserve and publish.
     * @param args Arguments to construct the events.
     */
    template <typename TEvent, typename... Args>
    void PublishMultipleEvents(size_t count, Args&&... args);

private:
    LockFreeEventQueue queue_;
    std::atomic<bool> stop_;
    std::thread worker_thread_;
};


template <typename T, class... Args>
std::pair<size_t, void*> IEventProcessor::ReserveEvent(Args&&... args) {
    // Forward the arguments to the queue's ReserveEvent function
    return queue_.ReserveEvent<T>(std::forward<Args>(args)...);
}

template <class T, class... Args>
IEventProcessor::ReservedEvent IEventProcessor::Reserve(Args&&... args) {
    // Reserve memory for the event in the queue
    const auto reservation = ReserveEvent<T>(std::forward<Args>(args)...);

    // If the reservation failed, return an invalid ReservedEvent
    if (!reservation.second) {
        return ReservedEvent(); //! TODO: handle this case
    }

    // Construct the event in the reserved memory location
    std::construct_at(reinterpret_cast<T*>(reservation.second), std::forward<Args>(args)...);

    // Return the reserved event, including the sequence number and pointer
    return ReservedEvent(reservation.first, reservation.second);
}



//! TODO: tobe further implemented
// std::vector<IEventProcessor::ReservedEvents> IEventProcessor::ReserveRange(size_t count) {
//     std::vector<IEventProcessor::ReservedEvents> reserved_events;

    
//     for (size_t i = 0; i < count; ++i) {
//         // Reserve a single event, for example, EventA
//         auto reserved_event = Reserve<EventA>(-1);
        
//         //!(const Integer sequence_number, void* const event, const size_t count, const size_t event_size);
//         IEventProcessor::ReservedEvents reservedEvents(reserved_event.GetSequenceNumber(), reserved_event);
//         // Add the valid reserved event to the collection

//         Integer sequence_number_;
//         void* event_;

//         reserved_events.push_back(reservedEvents);
//     }

//     return reserved_events;
// }




// Implementation of PublishEvent - Publishes a single event to the processor.
template <typename TEvent, typename... Args>
void PublishSingleEvents(IEventProcessor& processor, size_t count, Args&&... args)
{
    static_assert(std::disjunction_v<std::is_same<TEvent, EventA>,
                                     std::is_same<TEvent, EventB>,
                                     std::is_same<TEvent, EventC>,
                                     std::is_same<TEvent, EventD>,
                                     std::is_same<TEvent, EventE>,
                                     std::is_same<TEvent, EventF>>,
                  "Invalid event type!");

    // Loop over the number of events to be published
    for (size_t i = 0; i < count; ++i) {
        // Construct the event with the forwarded arguments
        TEvent event(std::forward<Args>(args)...);

        // Reserve the event
        auto reserved_event = processor.Reserve<TEvent>(event);

        // Check if the reservation was successful
        if (!reserved_event.IsValid()) {
            std::cerr << "Failed to reserve single event." << std::endl;
            continue;  // Proceed to the next iteration if reservation fails
        }

        // Commit the event to the processor
        processor.Commit(reserved_event.GetSequenceNumber());
    }
}






template <typename TEvent, typename... Args>
void IEventProcessor::PublishMultipleEvents(size_t count, Args&&... args) {
    static_assert(std::disjunction_v<std::is_same<TEvent, EventA>,
                                     std::is_same<TEvent, EventB>,
                                     std::is_same<TEvent, EventC>,
                                     std::is_same<TEvent, EventD>,
                                     std::is_same<TEvent, EventE>,
                                     std::is_same<TEvent, EventF>>,
                  "Invalid event type!");

    // Pass the event type TEvent and count to ReserveRange
    auto reserved_events_collection = ReserveRange<TEvent>(count);
    if (reserved_events_collection.empty()) {
        std::cerr << "Failed to reserve multiple events." << std::endl;
        return;
    }

    std::ranges::for_each(reserved_events_collection, [&](ReservedEvents& reserved_events) {
        if (!reserved_events.IsValid()) {
            std::cerr << "Failed to reserve event batch." << std::endl;
            return;
        }

        // Forward the provided arguments to Emplace
        for (size_t i = 0; i < reserved_events.Count(); ++i) {
            reserved_events.Emplace<TEvent>(i, std::forward<Args>(args)...);
        }

        Commit(reserved_events.GetSequenceNumber(), reserved_events.Count());
    });
}



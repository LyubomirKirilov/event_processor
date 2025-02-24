#pragma once

#include <atomic>
#include <array>
#include <chrono>
#include <variant>
#include <optional>
#include <iostream>
#include <thread>

#include "Event.h"

constexpr size_t BUFFER_SIZE = 1024;
constexpr size_t MAX_SPIN_COUNT = 1000;  // Number of spins before yielding CPU
constexpr size_t MAX_YIELD_COUNT = 10;   // Number of yields before sleeping

/**
 * @class LockFreeEventQueue
 * @brief A lock-free, multi-producer, single-consumer event queue.
 * 
 * This queue allows multiple threads to enqueue events efficiently while a single worker thread processes them.
 */
class LockFreeEventQueue {
public:
    /**
     * @brief Constructor initializes the queue.
     */
    LockFreeEventQueue();

    /**
     * @brief Reserve a slot for an event in the queue.
     * 
     * This function tries to reserve space for an event. If the queue is full, it waits (spins) until space is available.
     * 
     * @tparam T The event type.
     * @param event The event to be enqueued.
     * @return A pair of the reserved index and a pointer to the stored event.
     */
    template <typename T>
    std::pair<size_t, void*> ReserveEvent(T&& event);

    /**
     * @brief Commit an event to signal that it is ready to be processed.
     * 
     * @param sequence_number The index of the event that is now ready.
     */
    void Commit(size_t sequence_number);

    /**
     * @brief Enqueue an event at a reserved slot.
     * 
     * This function is used after reserving a slot to move the event into the queue.
     * 
     * @param index The index where the event should be stored.
     * @param event The event to store in the queue.
     */
    void Enqueue(size_t index, EventVariant&& event);

    /**
     * @brief Pop an event from the queue.
     * 
     * If an event is available and committed, it is removed from the queue and returned.
     * 
     * @return An optional EventVariant. If no event is available, std::nullopt is returned.
     */
    std::optional<EventVariant> Pop();

private:
    std::array<EventVariant, BUFFER_SIZE> buffer_;  ///< Circular buffer storing events.
    std::array<std::atomic<bool>, BUFFER_SIZE> committed_flags_ = {}; ///< Flags to indicate committed events.
    std::atomic<size_t> head_, tail_; ///< Atomic counters for head and tail.
};

template <typename T>
std::pair<size_t, void*> LockFreeEventQueue::ReserveEvent(T&& event) {
    size_t head, next_head;
    size_t spin_count = 0;
    size_t yield_count = 0;

    do {
        head = head_.load(std::memory_order_relaxed);
        next_head = (head + 1) % BUFFER_SIZE;

        if (next_head == tail_.load(std::memory_order_acquire)) {
            // Buffer is full, use adaptive spin-waiting
            if (spin_count < MAX_SPIN_COUNT) {
                ++spin_count;  // Increment spin counter
                continue;       // Try again, spinning
            } else if (yield_count < MAX_YIELD_COUNT) {
                std::this_thread::yield();  // Yield CPU to allow other threads to run
                ++yield_count;
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(100));  // Sleep to reduce CPU usage
                yield_count = 0;  // Reset yield count after sleep
            }
        }
    } while (!head_.compare_exchange_weak(head, next_head, std::memory_order_release));

    buffer_[head].emplace<T>(std::forward<T>(event));  // Store the event using move semantics
    return {head, static_cast<void*>(&buffer_[head])};
}

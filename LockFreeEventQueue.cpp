#include <iostream>

#include "LockFreeEventQueue.h"

LockFreeEventQueue::LockFreeEventQueue() : head_(0), tail_(0) {}

void LockFreeEventQueue::Commit(size_t sequence_number) {
    committed_flags_[sequence_number % BUFFER_SIZE].store(true, std::memory_order_release);
}

void LockFreeEventQueue::Enqueue(size_t index, EventVariant&& event) {
    buffer_[index % BUFFER_SIZE] = std::move(event);
}

std::optional<EventVariant> LockFreeEventQueue::Pop() {
    size_t tail, spin_count = 0, yield_count = 0;

    // Adaptive spin-waiting: we spin until we find an event, then perform yield or sleep as necessary
    do {
        tail = tail_.load(std::memory_order_relaxed);

        // Check if the queue is empty (tail == head)
        if (tail == head_.load(std::memory_order_acquire)) {
            if (spin_count < MAX_SPIN_COUNT) {
                ++spin_count;  // Increment spin counter
                continue;       // Spin to check again
            } else if (yield_count < MAX_YIELD_COUNT) {
                std::this_thread::yield();  // Yield CPU to allow other threads to run
                ++yield_count;
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(100));  // Sleep to reduce CPU usage
                yield_count = 0;  // Reset yield count after sleep
            }
        }

        // Check if the event is committed before processing
        if (!committed_flags_[tail % BUFFER_SIZE].load(std::memory_order_acquire)) {
            if (spin_count < MAX_SPIN_COUNT) {
                ++spin_count;
                continue;  // Spin again if not committed
            } else if (yield_count < MAX_YIELD_COUNT) {
                std::this_thread::yield();
                ++yield_count;
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(100));  // Sleep after excessive yields
                yield_count = 0;  // Reset yield count after sleep
            }
        }

        // If we've reached here, the event is available and committed
        EventVariant event = std::move(buffer_[tail % BUFFER_SIZE]);
        committed_flags_[tail % BUFFER_SIZE].store(false, std::memory_order_release); // Reset flag
        tail_.store((tail + 1) % BUFFER_SIZE, std::memory_order_release);  // Update the tail

        return event;
    } while (true);  // Keep looping until an event is found and returned
}


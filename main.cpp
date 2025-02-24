#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <variant>
#include <random>
#include <functional>

#include "IEventProcessor.h"
#include "Event.h"

// Define a function to publish single events
template <typename TEvent, typename... Args>
void worker(IEventProcessor& processor, size_t count, Args&&... args) {
    for (size_t i = 0; i < count; ++i) {
        // Correctly call PublishSingleEvents with the processor and count
        processor.PublishSingleEvents<TEvent>(processor, count, std::forward<Args>(args)...);
    }
}

// Define a function to process events from the queue (read thread)
void ProcessEvents(IEventProcessor& processor) {
    processor.ProcessEvents(); // Assuming this method processes events
}

int main() {
    IEventProcessor processor;

    const size_t numEvents = 10000000;  // 10 million events for demonstration

    std::vector<std::thread> threads;

    // Thread for writing events (publishing events)
    // Correctly call the worker function
    threads.push_back(std::thread(worker<Event<int>, int>, std::ref(processor), numEvents, 1));

    // Thread for reading events (processing events)
    threads.push_back(std::thread(ProcessEvents, std::ref(processor)));

    // Wait for both threads to finish
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}

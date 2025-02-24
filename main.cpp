#include <iostream>
#include <ranges>
#include <algorithm>

#include <memory>
#include <vector>

#include "IEventProcessor.h" //! TODO: consider actually including the interface?
#include "Event.h"



//! TODO: sequence_number_ shall be an atomic and threads should encrease it when they start new event


// Main Function (Example Usage)
int main() {
    std::unique_ptr<IEventProcessor> event_processor;
    event_processor = std::make_unique<IEventProcessor>();

    // queue 1 event ===========================================
    auto reserved_event = event_processor->Reserve<Event>(2);
    
    if (!reserved_event.IsValid()) {
    // ERROR: Reserve() failed ...
    } else {
        event_processor->Commit(reserved_event.GetSequenceNumber());
    }

    // queue multiple events  ===========================================
    auto reserved_events_collection = event_processor->ReserveRange(2); // It can reserve less items than requested! You should always check how many events have been reserved!
    if (reserved_events_collection.empty()) {
        // ERROR: ReserveRange() failed
    } else {
        std::ranges::for_each(reserved_events_collection, [&](IEventProcessor::ReservedEvents& reserved_events) {
            if (!reserved_events.IsValid()) {
                // Proper error handling
                std::cerr << "ERROR: Reserve() failed" << std::endl;  // Log an error message or handle accordingly
            } else {
                for (size_t i = 0; i < reserved_events.Count(); ++i) {
                    reserved_events.Emplace<Event>(i, static_cast<int>(i + 3));  // Emplacing the event
                }
        
                event_processor->Commit(reserved_events.GetSequenceNumber(), reserved_events.Count());  // Committing the events
            }
        });
    }
}
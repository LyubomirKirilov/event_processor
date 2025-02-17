// #include "EventExample.h"

// // queue 1 event
// auto reserved_event = event_processor->Reserve<Event>(2);
// if (!reserved_event.IsValid())
// {
// // ERROR: Reserve() failed ...
// }
// else
// {
// event_processor->Commit(reserved_event.GetSequenceNumber());
// }

// // queue multiple events
// auto reserved_events_collection = event_processor->ReserveRange(2); // It can reserve less items than requested! You should always check how many events have been reserved!
// if (reserved_events_collection.empty())
// {
// // ERROR: ReserveRange() failed
// }
// else
// {
// std::ranges::for_each(reserved_events_collection,
// [&](IEventProcessor::ReservedEvents& reserved_events)
// {
// if (!reserved_events.IsValid())
// {
// // ERROR: Reserve() failed
// }
// else
// {
// for (size_t i = 0; i < reserved_events.Count(); ++i)
// reserved_events.Emplace<Event>(i, static_cast<int>(i + 3));

// event_processor->Commit(reserved_events.GetSequenceNumber(), reserved_events.Count());
// }
// });
// }
#pragma once

//! TODO: sort the includes in all of the files
//! TODO: ake sure there are  no unused includes
//! TODO: consider making small functions here inline
//! TODO: mabe make a namespace for the IEvent processor and move the structures inside the namespace


#include "IEvent.h"

#include <atomic>  // For std::atomic
#include <vector>  // For std::vector
#include <thread>   //! TODO: mabe use more effecient version of therad like jthread or others

#include <cstdint>
#include <utility>
#include <memory>
#include <iostream>




// Lock-Free Queue Implementation
class LockFreeQueue {
    private:
        std::vector<IEvent*> buffer_;
        std::atomic<size_t> head_;
        std::atomic<size_t> tail_;
        size_t capacity_;
    
    public:
        LockFreeQueue();
        explicit LockFreeQueue(size_t capacity) : capacity_(capacity), buffer_(capacity), head_(0), tail_(0) {}
    
        bool Enqueue(IEvent* event) {
            size_t current_tail = tail_.load(std::memory_order_relaxed);
            size_t next_tail = (current_tail + 1) % capacity_;
    
            if (next_tail == head_.load(std::memory_order_acquire)) {
                return false;  // Queue is full
            }
    
            buffer_[current_tail] = event;
            tail_.store(next_tail, std::memory_order_release);
            return true;
        }
    
        IEvent* Dequeue() {
            size_t current_head = head_.load(std::memory_order_relaxed);
            if (current_head == tail_.load(std::memory_order_acquire)) {
                return nullptr;  // Queue is empty
            }
    
            IEvent* event = buffer_[current_head];
            head_.store((current_head + 1) % capacity_, std::memory_order_release);
            return event;
        }
};


class IEventProcessor
{
public:
    using Integer = int64_t;
    IEventProcessor ();
    ~IEventProcessor () = default;


    /** 
     * ORIGINAL FUNCTION
     * 
     */
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

        bool IsValid() const;

        private:
            Integer sequence_number_;
            void* event_;
    };


    /** 
     * ORIGINAL STRUCTURE
     * 
     */
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
        bool IsValid() const;

        private:
        Integer sequence_number_;
        void* events_;
        size_t count_;
        size_t event_size_;
    };


    /** 
     * ORIGINAL FUNCTION
     * 
     */
    //! TODO: consider imlpementing the template functions inside the header file
    template <typename T>
    std::pair<size_t, void* const> ReserveEvent() {
        // Dummy implementation for illustration
        //! TODO:
        static size_t sequence_number = 0;
        void* memory = ::operator new(sizeof(T));  // Allocate memory for T
        return {sequence_number++, memory};  // Return sequence number and memory
    }


    /** 
     * ORIGINAL FUNCTION TO BE USED
     * 
     */
    /// --- TODO: this should be the proper implementation, to be reverted
    // template <class T, class ...Args>
    // ReservedEvent Reserve(Args&&... args)
    // {
    //     const auto reservation = ReserveEvent<T>();
    //     if (!reservation.second)
    //         return ReservedEvent();

    //     std::construct_at(reservation.second, std::forward<Args>(args)...);
    //     return ReservedEvent(reservation.first, reservation.second);
    // }

    template <class T, class ...Args>
    ReservedEvent Reserve(Args&&... args)
    {
        const auto reservation = ReserveEvent<T>();
        if (!reservation.second)
            return ReservedEvent();

        // Cast void* to T* before using std::construct_at
        std::construct_at(static_cast<T*>(reservation.second), std::forward<Args>(args)...);
        
        return ReservedEvent(reservation.first, reservation.second);
    }


    /** 
     * ORIGINAL FUNCTION
     * 
     */
    std::vector<ReservedEvents> ReserveRange(const size_t size); // TODO: static vector?

    /** 
     * ORIGINAL FUNCTION
     * 
     */
    void Commit(const Integer sequence_number);

    /** 
     * ORIGINAL FUNCTION
     * 
     */
    void Commit(const Integer sequence_number, const size_t count);


    //! TODO: move those functions to be above
    //! TODO: make a callable that handles the events
    void Worker() {
        while (!stop_.load()) {
            IEvent* event = queue_.Dequeue();
            if (event) {
                event->Process();
                delete event;
            }
        }
    }


private:
    LockFreeQueue queue_;
    std::atomic<bool> stop_;
    std::thread worker_thread_;



    void StartProcessing() {
        worker_thread_ = std::thread(&IEventProcessor::Worker, this);
    }
};
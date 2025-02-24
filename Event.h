#pragma once

#include "IEvent.h"
#include <iostream>


class NetworkEvent : public IEvent {
public:
    explicit NetworkEvent( const int value) : value_(value) {}
    virtual ~NetworkEvent() override = default;

    NetworkEvent(const NetworkEvent&) = delete;
    NetworkEvent& operator=(const NetworkEvent&) = delete;
    
    NetworkEvent(NetworkEvent&&) = delete;
    NetworkEvent& operator=(NetworkEvent&&) = delete;


    //! TODO: why is this a virtual void, will there be different type of events?
    virtual void Process() override {
        std::cout << "Processing Event with value: " << value_ << std::endl;
    }
private:
    int value_;
};

class FileEvent : public IEvent {
    public:
        explicit FileEvent( const int value) : value_(value) {}
        virtual ~FileEvent() override = default;
    
        FileEvent(const FileEvent&) = delete;
        FileEvent& operator=(const FileEvent&) = delete;
        
        FileEvent(FileEvent&&) = delete;
        FileEvent& operator=(FileEvent&&) = delete;
    
    
        //! TODO: why is this a virtual void, will there be different type of events?
        virtual void Process() override {
            std::cout << "Processing FileEvent with value: " << value_ << std::endl;
        }
    private:
        int value_;
};

class UIEvent : public IEvent {
    public:
        explicit UIEvent( const int value) : value_(value) {}
        virtual ~UIEvent() override = default;
    
        UIEvent(const UIEvent&) = delete;
        UIEvent& operator=(const UIEvent&) = delete;
        
        UIEvent(UIEvent&&) = delete;
        UIEvent& operator=(UIEvent&&) = delete;
    
    
        //! TODO: why is this a virtual void, will there be different type of events?
        virtual void Process() override {
            std::cout << "Processing UIEvent with value: " << value_ << std::endl;
        }
    private:
        int value_;
};


class DatabaseEvent : public IEvent {
    public:
        explicit DatabaseEvent( const int value) : value_(value) {}
        virtual ~DatabaseEvent() override = default;
    
        DatabaseEvent(const DatabaseEvent&) = delete;
        DatabaseEvent& operator=(const DatabaseEvent&) = delete;
        
        DatabaseEvent(DatabaseEvent&&) = delete;
        DatabaseEvent& operator=(DatabaseEvent&&) = delete;
    
    
        //! TODO: why is this a virtual void, will there be different type of events?
        virtual void Process() override {
            std::cout << "Processing DatabaseEvent with value: " << value_ << std::endl;
        }
    private:
        int value_;
};
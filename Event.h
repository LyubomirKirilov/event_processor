#pragma once

#include <iostream>
#include <variant>

template <typename T>
class Event {
public:
    explicit Event(T value) : value_(value) {}
    void Process() const { std::cout << "Processing " << typeid(T).name() << ": " << value_ << std::endl; }
private:
    T value_;
};

using EventA = Event<int>;
using EventB = Event<int>;
using EventC = Event<int>;
using EventD = Event<int>;
using EventE = Event<int>;
using EventF = Event<int>;
using EventG = Event<int>;
using EventH = Event<int>;

using EventVariant = std::variant<std::monostate, EventA, EventB, EventC, EventD, EventE, EventF, EventG, EventH>;

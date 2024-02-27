#pragma once

#include <array>

// TODO: Implement the Quadruple struct, with the required specialisations
// - The general case should store types and have getters for each called `get_first`, `get_second`,
// etc
// - Implement once specialization if all types are ints. Store them in a publicly accessible
// `std::array`, again provide the getters.
// - Implement once specialization if all types are the same. Store them in a publicly accessible
// `std::array`, again provide the getters.
template <typename T, typename U, typename V, typename W> struct Quadruple {
public:
    T get_first() const { return first; }
    U get_second() const { return second; }
    V get_third() const { return third; }
    W get_fourth() const { return fourth; }

    Quadruple(T a, U b, V c, W d) : first(a), second(b), third(c), fourth(d) {}

private:
    T first;
    U second;
    V third;
    W fourth;
};


template <>
struct Quadruple<int, int, int, int> {
public:
    int get_first() const { return members.at(0); }
    int get_second() const { return members.at(1); }
    int get_third() const { return members.at(2); }
    int get_fourth() const { return members.at(3); }
    
    Quadruple(int a, int b, int c, int d) : members{a, b, c, d} {}
    std::array<int, 4> members;    
};


template <typename T>
struct Quadruple<T, T, T, T> {
public:
    T get_first() const { return members.at(0); }
    T get_second() const { return members.at(1); }
    T get_third() const { return members.at(2); }
    T get_fourth() const { return members.at(3); }

    Quadruple(T a, T b, T c, T d) : members{a, b, c, d} {}
    std::array<T, 4> members;    
};


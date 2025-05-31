// Enabling a homogeneous container of base classes storing child classes with CRTP rather than virtual calls
// Compile-time dispatch for everything => faster
// Inspired by a challenge found at work
#include <iostream>
#include <variant>
#include <vector>

/*
This is variation 1:
- Using a std::variant leaving runtime resolution to only type resolution of the variant
- It is quite maintainable with a variadic `MasterType`
- Benchmarks confirm a greater speed improvement over pure virtual functions
- The only caveat is that everything is statically typed, but grouping related objects for easy parameters, calling, etc. becomes MUCH easier!
- There can be more improvements, coming soon
  */

// CRTP base
template <typename Derived>
class IMaster {
public:
    void method() {
        static_cast<Derived*>(this)->method();
    }
};

// Implementations
class Impl1 : public IMaster<Impl1> {
public:
    void method() {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }
};

class Impl2 : public IMaster<Impl2> {
public:
    void method() {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }
};

template <typename... Ts>
class MasterType {
public:
    using VariantType = std::variant<Ts...>;
private:
    VariantType value;
public:
    template <typename Val>
    MasterType(Val&& obj) : value(std::forward<Val>(obj)) {}

    void method() {
        std::visit([](auto& obj) { obj.method(); }, value);
    }
};

using Master = MasterType<Impl1, Impl2>;

int main() {
    std::vector<Master> vec;
    vec.emplace_back(Impl1{});
    vec.emplace_back(Impl2{});

    for (auto& m : vec) {
        m.method();
    }

    return 0;
}

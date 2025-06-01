// Enabling a homogeneous container of base classes storing child classes with CRTP rather than virtual calls
// Compile-time dispatch for everything => faster
#include <iostream>
#include <any>
#include <functional>
#include <vector>

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

class Master {
    // Any is a way to support easy & safe type erasure in C++
    // So Master is really a wrapper around std::any with safeguards
    // The safeguards are to ensure that of course, it belongs under IMaster
    std::any object;
    std::function<void(std::any&)> method_fn;
    // The above two members will entail a runtime dispatch via a function poitner jump
    // Will not be as costly as virtual

public:
    template <typename T,
              typename Decayed = std::decay_t<T>,
              typename = std::enable_if_t<std::is_base_of_v<IMaster<Decayed>, Decayed>>>
    Master(T&& obj)
        : object(std::make_any<Decayed>(std::forward<T>(obj)))
    {
       // Lambda is compiled per instantiation of T, so type-specific & statically compiled
        method_fn = [](std::any& any_obj) {
            auto& actual = std::any_cast<Decayed&>(any_obj);
            actual.method();
        };
    }

    void method() {
        method_fn(object);
    }
};

int main() {
    std::vector<Master> vec;
    vec.emplace_back(Impl1{});
    vec.emplace_back(Impl2{});

    for (auto& m : vec) {
        m.method();
    }

    return 0;
}

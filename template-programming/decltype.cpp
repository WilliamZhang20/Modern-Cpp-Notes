#include <iostream>
#include <typeinfo>

struct A { double x; };
 
template<typename T, typename U>
// Example below, also with trailing return type
auto add(T t, U u) -> decltype(t + u) // return type depends on template parameters
{
    return t + u;
}

int main() {
    A obj;
    A* a = &obj;
    a->x = 5.0;
    
    decltype(a->x) y;       // type of y is double (declared type)
    decltype((a->x)) z = a->x; // type of z is const double& (lvalue expression)
 
    unsigned v1 = add(1, 2u);
    double v2 = add(1.5, 2u);
    
    // On some compilers, output of the below is "d" for double, "j" for unsigned int
    std::cout << v2 << " is type of " << typeid(decltype(v2)).name() << "\n";

    z = 10.0;
    std::cout << "decltype preserved references, expect 10: " << a->x << "\n";

    a->x = 15.4;
    std::cout << "Second try, expect 15.4: " << a->x << "\n";
    return 0;
}
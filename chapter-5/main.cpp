#include <iostream>

// c++11 没有make_unique
template <typename T, typename... Args>
std::unique_ptr my_make_unique(Args... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int main() {
    
}

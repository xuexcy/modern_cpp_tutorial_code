#include <functional>
#include <iostream>
#include <type_traits>

#include "foo.h"

class HasDestructor {
public:
    HasDestructor() = default;
    ~HasDestructor() = default;
};

class NoDestructor {
public:
    NoDestructor() = default;
    ~NoDestructor() = default;
};

// 1.1 弃用
void discard() {
    // #1. 不再允许使用字面值常量赋值给 char*
    // warning: ISO C++11 does not allow conversion from string literal to 'char *'
    char* str = "hello world;";
    // use const char* or auto
    const char* str_2 = "hello world";

    // #2. bool 不能 ++
    bool yes{false};
    // ++yes;

    // #3: 带析构函数的class，弃用为其生成拷贝或赋值构造函数，因为可能造成析构函数二次 delete
    // output: 1 （这里是弃用，所以还是1)
    std::cout << std::is_copy_constructible_v<HasDestructor> << std::endl;
    // output: 1
    std::cout << std::is_assignable_v<HasDestructor, HasDestructor> << std::endl;
    // output: 1
    std::cout << std::is_copy_constructible_v<NoDestructor> << std::endl;
    // output: 1
    std::cout << std::is_assignable_v<NoDestructor, NoDestructor> << std::endl;

}

// 1.2 与c的兼容性
void compatible_c() {
    [out=std::ref(std::cout << "Result from C code: " << add(1, 2))] () {
        out.get() << ".\n";
    }();
}

int main() {
    discard();
    compatible_c();
}

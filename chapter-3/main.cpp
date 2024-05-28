#include <cassert>
#include <functional>
#include <iostream>
#include <type_traits>

#include "A.h"

void foo(int x, int y, int z) {
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "z: " << z << std::endl;
}

void reference(std::string& s) {
    std::cout << "Left value: " << s << std::endl;
}

void reference(std::string&& s) {
    std::cout << "Right value: " << s << std::endl;
}

int main() {
    auto bind_foo = std::bind(foo, std::placeholders::_1, 2, 3);
    bind_foo(1);
    auto bind_foo_2 = std::bind(foo, 2, std::placeholders::_1, 3);
    bind_foo_2(1);


    // 字符串字面值是一个 const char[N], 存储与静态数据区，这是一个左值
    // 1. left 将数据从静态数据区拷贝到栈上, 所以"01234"和left的地址不同
    std::cout << &("01234") << std::endl;
    const char left[6] = "01234";
    std::cout << &left << " " << left << std::endl;
    assert(&"01234" != &left);
    // 2. left_2 和 left_3可以直接引用"01234"的地址, 所以"01234"和left_2, left_3的地址相同
    const char (&left_2)[6] = "01234";
    const char (&left_3)[6] = "01234";
    std::cout << left_2 << std::endl;
    std::cout << &"01234" << " " << &left_2 << " " << &left_3 << std::endl;
    assert(&"01234" == &left_2);
    assert(&left_2 == &left_3);
    // 3. decltype是通过引用读取的"01234"来获取其类型,所以是const char(&)[6]
    static_assert(std::is_same_v<decltype("01234"), const char(&)[6]>, "not same");
    // 4. const char[6](如left变量)在栈上,是一个栈上的变量,而"01234"在静态数据区
    static_assert(std::is_same_v<decltype(left), const char [6]>, "not same");
    static_assert(!std::is_same_v<decltype("01234"), const char [6]>, "same");
    static_assert(!std::is_same_v<decltype(left), decltype(left_2)>, "same");
    // 5. NO, "01234" 是左值,不能被右值引用
    // const char (&&left_2)[6] = "01234";
    // 6. left_4从静态数据区拷贝数据
    char left_4[6] = "01234";
    // 7. NO, left_5 不是 const, 不能引用"01234"
    // char (&left_5)[6] = "01234";
    // 8. NO, left_6 是右值引用, "01234"是左值
    // const char (&&left_6)[6] = "01234";

    // const& 可以引用一个字面量(也就是说本来1是一个右值int，但是const&这个左值引用可以引用它)
    const int (&left_i1) = 1;
    const int (&left_i2) = 1;
    // int(1) 并不像字符串一样在静态数据区，所以两个1的地址不同
    std::cout << &left_i1 << " " << &left_i2 << std::endl;
    assert(&left_i1 != &left_i2);
    // 本来1就是一个右值
    const int (&&left_i3) = 1;

    // 隐式转换:数组可以转换成对应类型的指针
    // const char[] -> const char*
    const char* p = "123";
    // "123"先被转换成const char*, 然后这个const char*没有名字，是个右值
    // 与p_ref就可以是const char*的右值引用
    const char*&& p_ref = "123";


    std::string lv1 = "abc";
    std::string&& rv1 = std::move(lv1);
    std::cout << rv1 << std::endl;
    // 常量引用可以引用右值，以延长临时变量的生命周期
    const std::string& lv2 = rv1 + rv1;
    std::cout << lv2 << std::endl;

    std::string&& rv2 = rv1 + rv1;
    std::cout << rv2 << std::endl;
    rv2 += "Test";
    std::cout << rv2 << std::endl;

    reference(rv2);  // left, rv2是右值引用类型，其本身是一个左值变量
    A obj = return_rvalue(false);
    std::cout << "obj:\n";
    std::cout << obj.p << std::endl;
    std::cout << *obj.p << std::endl;
    return 0;
}

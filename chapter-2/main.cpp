#include <ctime>
#include <format>
#include <initializer_list>
#include <iostream>
#include <type_traits>
#include <vector>

void foo(char*) { std::cout << "foo(char*)\n"; }
void foo(int) { std::cout << "foo(int)\n"; }

void constant_nullptr() {
    if (std::is_same_v<decltype(NULL), decltype(0)>) {
        std::cout << "NULL == 0" << std::endl;
    }
    if (std::is_same_v<decltype(NULL), decltype((void*)0)>) {
        std::cout << "NULL == (void *)0" << std::endl;
    }
    if (std::is_same_v<decltype(NULL), std::nullptr_t>) {
        std::cout << "NULL == nullptr" << std::endl;
    }

    foo(0); // foo(int)
    // can't compile
    // foo(NULL);
    foo(nullptr); // foo(char*)
}

int len_foo() {
    int i = 2;
    return i;
}

constexpr int len_foo_constexpr() {
    return 5;
}

// c++ 11 constexpr 函数支持递归
constexpr int fibonacci(const int n) {
    return (n == 1 || n == 2) ? 1: fibonacci(n-1) + fibonacci(n-2);
}
// c++ 14 constexpr 函数支持循环、分支
// 这段代码在c++11 编译不通过
constexpr int fibonacci_2(const int n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    return fibonacci(n-1) + fibonacci(n-2);
}
void constant_constexpr(int i) {
    #define LEN 10
    char arr_1[10];
    char arr_2[LEN];

    int len = i;
    std::cout << len << std::endl;
    char arr_3[len];  // 现在的编译器支持

    const int len_2 = len + 1;
    constexpr int len_2_constexpr = 1 + 2 + 3;
    // char arr_4[len_2];
    char arr_5[len_2_constexpr];

    char arr_6[len_foo_constexpr() + 1];

    std::cout << fibonacci(10) << std::endl;
}
// 2.1 常量
void test_constant() {
    constant_nullptr();
    std::cout << std::time(nullptr) << std::endl;
    constant_constexpr(std::time(nullptr) % 10000);
}

class MagicFoo {
public:
    std::vector<int> vec;
    MagicFoo(std::initializer_list<int> list) {
        append(list);
    }
    void append(std::initializer_list<int> list) {
        for (auto elem : list) {
            vec.push_back(elem);
        }
    }
    void print() {
        std::cout << "magic:";
        for (auto elem : vec) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
};

// 2.2 变量及其初始化
void test_variable_initialize() {
    // c++ 17
    if (int i = 0; i < 2) {}
    MagicFoo magic_foo = {1, 2, 3, 4, 5};
    magic_foo.print();
    magic_foo.append({6, 7, 8});
    magic_foo.print();
}

// c++ 14 只能在lambda函数中用auto
// c++ 20 可以在一般函数中使用
int add_20(auto x, auto y) {
    return x + y;
}

/*
// c++11编译器读到 decltype(t + u) 时，t、u的类型还不确定，所以无法推导出返回类型
template <typename T, typename U>
decltype(t + u) add(T t, U u) {
    return t + u;
}*/

// c++11 尾类型推导 trailing return type
template <typename T, typename U>
auto add2(T t, U u) -> decltype(t + u) {
    return t + u;
}

// c++ 14 普通函数直接推导返回值类型
template <typename T, typename U>
auto add3(T t, U u) {
    return t + u;
}

std::string look_up() {
    return "1";
}
// c++ 11 返回参数转发
decltype(auto) look_up_auto() {
    return look_up();
}

// 2.3 类型推导
void test_deducing_types() {
    auto add_14 = [](auto x, auto y) -> int {
        return x + y;
    };

    auto x = 1;
    auto y = 2;
    decltype(x + y) z;

    // true
    std::cout << std::is_same_v<decltype(x), int> << std::endl;
    // false
    std::cout << std::is_same_v<decltype(x), float> << std::endl;
    // true
    std::cout << std::is_same_v<decltype(x), decltype(z)> << std::endl;

    std::cout << look_up_auto() << std::endl;
}

template <class T>
auto print_type_info(const T& t) {
    if constexpr (std::is_integral<T>::value) {
        return t + 1;
    } else {
        return t + 0.001;
    }
}
/*
int print_type_info(const int& t) {
    return t + 1;
}
double print_type_info(const double& t) {
    return t + 1;
}
*/

void test_control_flow() {
    std::cout << print_type_info(1) << std::endl;
    std::cout << print_type_info(1.0) << std::endl;
}

// 递归解变长模板
template <class T>
void printf1(T value) {
    std::cout << value << std::endl;
}
template <class T, class... Ts>
void printf1(T value, Ts... args) {
    printf1(value);
    // std::cout << value << std::endl;
    printf1(args...);
}

// c++ 17 使用 constexpr简化解变长模板
template <class T, class... Ts>
void printf2(T value, Ts... args) {
    std::cout << value << std::endl;
    if constexpr (sizeof...(args) > 0) { printf2(args...); }

}

template <class T, class... Ts>
void printf3(T value, Ts... args) {
    std::cout << value << std::endl;
    (void) std::initializer_list<T>{
        (
            [&args] {
                std::cout << args << std::endl;
            }(),
            value
        )...
    };
}

template <class T, class U>
class MagicType {
public:
    T t;
    U u;
};

// 变长模板
// NO, 不同的模板类型会实例化多个同名的FakeDarkMagic
// FakeDarkMagic 这个模板既声明了T，又使用了T
// template <class T>
// typedef MagicType<std::vector<T>, std::string> FakeDarkMagic;
template <class... T>
void print_size(T... args) {
    std::cout << sizeof...(args) << std::endl;
}

// 折叠表达式
template <class... T>
auto sum(T... args) {
    return (args + ...);
}

// 非类型(也就是字面量)模板参数
template <typename T, int BufSize>
class buffer_t {
public:
	T& alloc();
	void free(T& item);
    void size() {
        std::cout << sizeof(data)/sizeof(T) << std::endl;
        std::cout << BufSize << std::endl;
    }
private:
	T data[BufSize];
};

// 编译器辅助类型推导
template <auto value>
void auto_print() {
    std::cout << value << std::endl;
}

struct Temp {
    int x;
    int y;
    int z;
    friend std::ostream& operator <<(std::ostream& os, const Temp& temp);
};

std::ostream& operator <<(std::ostream& os, const Temp& temp) {
    os << std::format("Temp x:{}, y:{}, z:{}", temp.x, temp.y, temp.z);
    return os;
}

template <class T>
void test_template() {
    // YES, V只是使用了T，test_template声明了T
    // 当有多个不同类型实例化模板是, V 只能在对应的实例里面使用，不存在名字冲突
    // 比如 test_template<T1>::V, test_template<T2>::V，它们有不同的作用域
    typedef std::vector<T> V;
    using VV = std::vector<T>;  // YES
    V v;
    v.push_back(1.0);
    VV vv;
    vv.push_back(2.0);
    print_size(1);
    print_size(1, 2);
    print_size("string", 1, 2);

    printf1(1, "x");
    printf2(2, "y");
    printf3(3, "z");

    std::cout << sum(1) << std::endl;
    std::cout << sum(1, 2, 3, 4, 5) << std::endl;

    buffer_t<int, 100> buf; // 100 作为模板参数
    buf.size();
    // BufSize 必须是字面量, 因为模板是在编译期实例化的
    // buffer_t<int, sum(1, 2, 3)> buf2;

    auto_print<1>();
    auto_print<float(2.3)>();
    auto_print<'x'>();
    constexpr char x{'x'};
    auto_print<x>();
    const char y{'y'};
    auto_print<y>();

    char z{'z'};
    // auto_print<z>();  // 'char z' is not const

    constexpr Temp temp{1, 2, 3};
    auto_print<temp>();
    auto_print<Temp{4, 5, 6}>();

    // constexpr std::string c_s("xcy");
    // auto_print<c_s>();
}

class Base {
    int value1;
    int value2;
public:
    Base() {
        value1 = 1;
    }
    // Base(int value): Base(), value2(value) { 不行，委托构造和成员初始化不能同时进行
    Base(int value): Base() {  // 委托构造
        value2 = value;
    }
    virtual void foo1(int ) {}
    virtual void foo2() final {}
};

class Derive : public Base {
    int value2;
public:
    using Base::Base;  // 继承构造
    // override显示虚函数重载
    virtual void foo1(int) override {}  // YES, 通过使用override，编译器检查父类是否有相同签名函数的虚函数
    virtual void foo1(float) {}  // YES, 没有override，编译器不会检查父类，这里相当于Derive重新定义了一个foo函数
    // virtual void foo1(float) override;  // NO, 编译器检查到父类没有foo1(float), 编译不通过

    // 如果不使用override
    // 1. Derive 定义了一个函数，恰好和Base的同名了，但定义Derive的开发并不知道
    // 2. Derive 本来打算重载Base的virtual函数的，结果Base把这个virtual函数删了，
    //  那么在Derive里这个函数就重重载虚函数变成了普通函数
};

class Derive2 final: public Base {
    // 将 Derive2 定义成 final
};
/*
class Derive3 : public Derive2 {
    // 编译器: cannot derive from 'final' base 'Derive2' in derived type 'Derive3
    // 非法, 因为Derive2已经是final了，不能被继承
};*/

/*
class Derive4 : public Base {
public:
    // 非法, 因为 Base::foo2 已经被定义成final了，不能重载
    // 编译器: error: virtual function 'virtual void Derive4::foo2()' overriding final function
    virtual void foo2() override {}
};
*/

// 强枚举类型
enum class MyEnum {
    a,
    b,
    c,
    d = 100,
    e = 200
};

template <typename T>
std::ostream& operator<<(
        typename std::enable_if<
            std::is_enum<T>::value,
            std::ostream>::type& stream,
        const T& t) {
    return stream << static_cast<typename std::underlying_type<T>::type>(t);
}
void test_object_oriented() {
    Base b1;
    Base b2(2);
    Derive d1;
    Derive d2(2);

    std::cout << "MyEnum::c " << static_cast<typename std::underlying_type<MyEnum>::type>(MyEnum::c) << std::endl;
    std::cout << "MyEnum::d " << static_cast<typename std::underlying_type<MyEnum>::type>(MyEnum::d) << std::endl;
    std::cout << "MyEnum::e " << (MyEnum::e) << std::endl;
}
int main() {
    test_constant();
    test_variable_initialize();
    test_deducing_types();
    test_control_flow();
    test_template<int>();
    test_object_oriented();
}

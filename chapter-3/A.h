#include <iostream>
class A {
public:
    int* p;
    A(): p(new int(1)) {
        std::cout << "构造 " << p << std::endl;
    }
    A(A& a): p(new int(*a.p)) {
        std::cout << "拷贝 " << p << std::endl;
    }
    A(A&& a): p(a.p) {
        a.p = nullptr;
        std::cout << "移动 " << p << std::endl;
    }
    ~A() {
        std::cout << "析构 " << p << std::endl;
        delete p;
    }
};

A return_rvalue(bool test) {
    A a, b;
    std::cout << "a.p: " << a.p << std::endl;
    std::cout << "b.p: " << b.p << std::endl;
    if (test) {
        return a;
    } else {
        return b;
    }
}

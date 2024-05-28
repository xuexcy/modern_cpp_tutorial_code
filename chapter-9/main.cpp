#include <iostream>

void may_throw() {
    throw true;
}
// if throw exception, compiler call `std::terminal()`
void no_throw() noexcept;

std::string operator"" _wow1(const char* wow1, size_t len) {
    return std::string(wow1) + "woooow, amazing";
}

std::string operator"" _wow2(unsigned long long i) {
    return std::to_string(i) + "wooow, amazing";
}

struct Storage {
    char a;
    int b;
    double c;
    long long d;
};

struct alignas(std::max_align_t) AlignasStorage {
    char a;
    int b;
    double c;
    long long d;
};
int main() {
    auto non_block_throw = [] {
        may_throw();
    };
    auto block_throw = []() noexcept {
        no_throw();
    };
    // false true false true
    std::cout << std::boolalpha
        << "may_throw() noexcept? " << noexcept(may_throw()) << std::endl
        << "no_throw() noexcept? " << noexcept(no_throw()) << std::endl
        << "lmay_throw() noexcept? " << noexcept(non_block_throw()) << std::endl
        << "lno_throw() noexcept? " << noexcept(block_throw()) << std::endl;

    std::cout << "abc"_wow1 << std::endl;
    std::cout << 1_wow2 << std::endl;

    // 8
    std::cout << alignof(Storage) << std::endl;
    // 16
    std::cout << alignof(AlignasStorage) << std::endl;
    // 16
    std::cout << alignof(std::max_align_t) << std::endl;
    return 0;

}

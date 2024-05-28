#include <array>
#include <iostream>
#include <tuple>
#include <variant>
#include <vector>

auto get_student(int id) {
    if (0 == id) {
        return std::make_tuple(3.8, 'A');
    } else if (1 == id) {
        return std::make_tuple(2.9, 'C');
    } else if (2 == id) {
        return std::make_tuple(1.7, 'D');
    } else {
        return std::make_tuple(0.0, 'D');
    }
}

void print_student(auto student) {
    std::cout << "GPA: " << std::get<0>(student) << " Grade: " << std::get<1>(student) << std::endl;
    std::cout << "GPA: " << std::get<double>(student) << " Grade: " << std::get<char>(student) << std::endl;
}

template <size_t n, typename... T>
constexpr std::variant<T...> _tuple_index(const std::tuple<T...>& tpl, size_t i) {
     if constexpr (n < sizeof...(T)) {
        if (i == n) {
            return std::variant<T...>{ std::in_place_index<n>, std::get<n>(tpl) };
        } else {
            return _tuple_index<n + 1>(tpl, i);
        }
    } else {
        throw std::out_of_range("越界");
    }
}

template <typename... T>
constexpr std::variant<T...> tuple_index(const std::tuple<T...>& tpl, size_t i) {
    return _tuple_index<0>(tpl, i);
}

template <typename T0, typename ... Ts>
std::ostream & operator<< (std::ostream & s, std::variant<T0, Ts...> const & v) {
    std::visit([&](auto && x){
        s << x;
    }, v);
    return s;
}

int main() {
    std::vector<int> v;
    // 0 0
    std::cout << v.size() << " " << v.capacity() << std::endl;
    v.push_back(1);
    v.push_back(1);
    v.push_back(1);
    // 3 4
    std::cout << v.size() << " " << v.capacity() << std::endl;
    v.clear();
    // 0 4, clear后不会归还申请的内存
    std::cout << v.size() << " " << v.capacity() << std::endl;

    std::array<int, 4> arr = {1, 2 ,3, 4};
    constexpr int len{6};
    std::array<int, len> arr2 = {1, 2, 3, 4, 5, 6};

    auto student = get_student(0);
    print_student(student);

    double gpa;
    char grade;
    std::tie(gpa, grade) = get_student(1);
    std::cout << "GPA: " << gpa << " Grade: " << grade << std::endl;
    int i = 0;
    std::cout << tuple_index(student, i) << std::endl;
}

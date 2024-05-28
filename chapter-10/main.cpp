#include <iostream>

template <typename T>
requires Sortable<T>
void sort(T& t);

// 简写
template <Sortable T>
void sort(T& t);

void sort(Sortable& c);

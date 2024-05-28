# 编译 c 语言代码, 得到 foo.o
gcc -c foo.c

clang++ main.cpp foo.o -std=c++2a -o main

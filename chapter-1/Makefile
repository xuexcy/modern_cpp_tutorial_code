C = gcc
CXX = clang++

SOURCE_C = foo.c
OBJECT_C = foo.o

SOURCE_CXX = main.cpp

TARGET = main
LDFLAG_COMMON = -std=c++2a

all:
	$(C) -c $(SOURCE_C)
	$(CXX) $(SOURCE_CXX) $(OBJECT_C) $(LDFLAG_COMMON) -o $(TARGET)
clean:
	rm -rf *.o $(TARGET)

Eclipse:
Установить опции --­­std=c++11 ­-pthread (project­>properties­>c++ build­>settings­> GCC C++ Compiler ­> command)
Установить опцию -­pthread (project­>properties­>c++ build­>settings­> GCC C++ Linker)

Компилятор g++
Опция -g – отладка 
g++ --­­std=c++11 ­-pthread –g code.cpp –o code.out
./code.out
gdb code.out
b main.cpp:10, r, info threads, thread 5…

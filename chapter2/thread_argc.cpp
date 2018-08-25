#include<iostream>
#include<thread>

void f(int i,std::string const &s);
std::thread t(f,3,"hello");
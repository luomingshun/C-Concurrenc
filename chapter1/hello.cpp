#include<iostream>
#include<thread>
#include<unistd.h>
void hello() {
    sleep(2);
    std::cout<<"hello concurrent world\n";
}
int main() {
    std::thread t(hello);
    t.join();
    return 0;
}
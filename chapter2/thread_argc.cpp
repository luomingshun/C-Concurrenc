#include<iostream>
#include<thread>

void f(int i,std::string const &s);
std::thread my_t(f,3,"hello");

void oops(int var) {
    char buffer[1024];
    sprintf(buffer,"%i",var);
    std::thread t(f,3,buffer);
    t.detach();
}

void not_oops(int var) {
    char buffer[1024];
    sprintf(buffer,"%i",var);
    std::thread t(f,3,std::string(buffer));
    t.detach();
}

//sometimes we need a reference
void update_data(int id,int &data);
void oops_again(int id) {
    int data;
    std::thread t(update_data,id,std::ref(data));
    //displays_status();
    t.join();
    //process_data(data);
}

//------------------------
class X{
public:
    void do_lengthy_work();
};
X my_x;
std::thread t(&X::do_lengthy_work,&my_x);

class X1
{
public:
    void do_lengthy_work(int);
};
X1 my_x1;
int num(1);
std::thread t1(&X1::do_lengthy_work,&my_x1,num);

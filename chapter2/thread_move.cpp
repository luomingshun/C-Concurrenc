#include<iostream>
#include<thread>
#include<vector>
#include<unistd.h>
void some_function();
void some_other_function();

std::thread t1(some_function);
std::thread t2 = std::move(t1);
t1 = std::thread(some_other_function);
std::thread t3;
t3 = std::move(t2);
t1 = std::move(t3);

std::thread f() {
    return std::thread(some_function);
}
std::thread g() {
    std::thread t(some_other_function);
    return t;
}


class scoped_thread 
{
private:
    std::thread t_;
public:
    explicit scoped_thread(std::thread t):t_(std::move(t)) {
        if(!t_.joinable()) {
            throw std::logic_error("no thread");
        }
    }
    ~scoped_thread() {
        t_.join();
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};

struct func
{
    int &i_;
    func(int &i):i_(i){}
    void operator()() {
        for(unsigned j=0;j<10000;++j)
        {
            sleep(1);
            //do_loopwork(i_);//maybe procuce an error! not illegal reference
        }
    }
};
void do_some_thing();
void f1() {
    int var=1;
    scoped_thread t(std::thread(func(var)));
    do_some_thing();
}


//----------
void do_work(unsigned id);

void f2() {
    std::vector<std::thread> threads;
    for(unsigned i=0;i<20;++i) {
        threads.push_back(std::thread(do_work,i));
    }
    std::for_each(threads.begin(),threads.end(),
        std::mem_fn(&std::thread::join));
}
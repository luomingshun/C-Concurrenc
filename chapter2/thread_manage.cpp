#include<iostream>
#include<thread>
#include<unistd.h>
//call function
void do_some_work() {
    sleep(1);
    std::cout<<"do some work"<<std::endl;
}
void do_something() {
    sleep(2);
    std::cout<<"do something"<<std::endl;
}
void do_loopwork(int i) {
    sleep(3);
    std::cout<<"do loopwork "<<i<<std::endl;
}

std::thread t1(do_some_work);

//call class
//
class background_task
{
public:
    void operator()() const
    {
        do_something();
    }
};
background_task fun;
std::thread t2(fun);

//this is a bad example!
//this is a function named "t3" who  returns a std::thread 
std::thread t3(background_task());

//we can make threads by these ways
std::thread t4((background_task()));
std::thread t5{background_task()};

//or in this case
std::thread t6([]{
    do_some_work();
    do_something();
});

//***************************************************************
//another example
struct func
{
    int &i_;
    func(int &i):i_(i){}
    void operator()() {
        for(unsigned j=0;j<10000;++j)
        {
            sleep(1);
            do_loopwork(i_);//maybe procuce an error! not illegal reference
        }
    }
};

void oops() {
    int var = 1;
    func myfunc(var);
    std::thread my_thread(myfunc);
    my_thread.detach();//detach the thread
}

void f()
{
    int var =1;
    func myfunc(var);
    std::thread t(myfunc);
    try {
        do_something();
    }
    catch(...)
    {
        t.join();
        throw;
    }
    t.join();
}

//RAII
class thread_guard
{
private:
    std::thread &t_;
public:
    explicit thread_guard(std::thread &t):t_(t) {}
    ~thread_guard() {
        if(t_.joinable()) {
            t_.join();
        }
    }
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
};

void f1() {
    int var=1;
    func myfunc(var);
    std::thread t(myfunc);
    thread_guard g(t);
    do_something();
}

//concurrent edit document
//fake code
void edit_document(std::string const &filename) {
    open_document_and_display_gui(filename);
    while(!done_editing()) {
        user_command cmd = get_user_input();
        if(cmd.type == open_new_document) {
            std::string const new_name=get_filename_form_user();
            std::thread t(edit_document,new_name);
            t.detach();
        }
        else {
            process_user_input(cmd);
        }
    }
}
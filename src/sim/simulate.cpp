#include <iostream>

#include "simulate.h"


int main()
{
    double pause_seconds = 1;

    // Thread with no args
    ThreadUtil::Thread<ThreadUtil::Empty> my_empty_thread(10.0, std::bind(&callback_empty_func, std::placeholders::_1));
    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));
    my_empty_thread.join();
    std::cout << std::endl;

    ThreadArgsTest args;

    // Thread with args
    args.set_counter(0);
    std::cout << "Mutex and args test" << std::endl;
    ThreadUtil::Thread<ThreadArgsTest> my_arg_thread(10.0, std::bind(&callback_args_func, std::placeholders::_1), &args);
    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));
    std::cout << "Main counter val: " <<  args.get_counter() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));
    args.reset();
    args.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));
    args.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));
    my_arg_thread.join();
    std::cout << std::endl;

    // Thread with args and constructor/destructor
    std::cout << "With constructor and destructor for the thread" << std::endl;
    ThreadUtil::Thread<ThreadArgsTest> my_arg_adv_thread(10.0, std::bind(&callback_args_func, std::placeholders::_1), &args, std::bind(&constructor, std::placeholders::_1), std::bind(&destructor, std::placeholders::_1));
    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));
    my_arg_adv_thread.join();
    std::cout << "Final value: " << args.get_counter() << std::endl;
    std::cout << std::endl;

    // Threads with shared queue
    typedef int queue_type;
    ThreadQueueTest<queue_type> queue_args;
    queue_args.set_counter_1(0);
    queue_args.set_counter_2(0);
    std::cout << "Two threads sharing a queue running at different rates" << std::endl;
    ThreadUtil::Thread<ThreadQueueTest<queue_type>> my_queue_1_thread(10.0, std::bind(&callback_queue_1_func, std::placeholders::_1), &queue_args);
    ThreadUtil::Thread<ThreadQueueTest<queue_type>> my_queue_2_thread(5.0, std::bind(&callback_queue_2_func, std::placeholders::_1), &queue_args);
    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));
    my_queue_1_thread.join();
    my_queue_2_thread.join();
    std::cout << "Final value Counter1: " << queue_args.get_counter_1() << std::endl;
    std::cout << "Final value Counter2: " << queue_args.get_counter_2() << std::endl;


    return 0;
}



//
void callback_empty_func(ThreadUtil::Empty*)
{
    std::cout << "Callback!" << std::endl;
}

//
void callback_args_func(ThreadArgsTest* args)
{
    args->lock();

    args->increment();
    std::cout << "Counter: " << args->get_counter() << std::endl;

    args->unlock();
}
void constructor(ThreadArgsTest* args)
{
    args->set_counter(-10);
}
void destructor(ThreadArgsTest* args)
{
    args->set_counter(-10);
}

//
void callback_queue_1_func(ThreadQueueTest<int>* args)
{
    args->increment();
    args->push(args->get_counter_1());
}
void callback_queue_2_func(ThreadQueueTest<int>* args)
{
    args->set_counter_2(args->pop());
    std::cout << "Counter2: " << args->get_counter_2() << std::endl;
}
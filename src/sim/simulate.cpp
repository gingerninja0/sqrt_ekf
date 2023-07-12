#include <iostream>
#include <type_traits>

#include <Eigen/Dense>

#include "../util/thread_util.h"

struct ThreadArgsTest
{
    private:
        int counter;
    public:
        void increment(void)
        {
            counter++;
        }
        void reset(void)
        {
            counter = 0;
        }
        int get_counter(void) const
        {
            return counter;
        }
        void set_counter(const int val)
        {
            counter = val;
        }
};


void callback_empty_func(ThreadUtil::Empty*);

void callback_args_func(ThreadArgsTest*);

int main()
{
    std::cout << "Hello" << std::endl;

    std::function<void(ThreadUtil::Empty*)> callback_empty;
    callback_empty = std::bind(&callback_empty_func, std::placeholders::_1);

    ThreadUtil::Thread<ThreadUtil::Empty> my_empty_thread(10.0, callback_empty);

    double pause_seconds = 1;

    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));

    my_empty_thread.join();

    // Thread with args

    ThreadArgsTest args;
    args.set_counter(0);

    std::function<void(ThreadArgsTest*)> callback_args;
    callback_args = std::bind(&callback_args_func, std::placeholders::_1);

    ThreadUtil::Thread<ThreadArgsTest> my_arg_thread(10.0, callback_args, &args);

    pause_seconds = 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));

    std::cout << "Main counter val: " <<  args.get_counter() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));

    args.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * pause_seconds)));

    my_arg_thread.join();



    return 0;
}




void callback_empty_func(ThreadUtil::Empty*)
{
    std::cout << "Callback!" << std::endl;
}

void callback_args_func(ThreadArgsTest* args)
{
    args->increment();
    std::cout << "Counter: " << args->get_counter() << std::endl;
}

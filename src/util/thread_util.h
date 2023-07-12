#pragma once

#include <atomic>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>

namespace ThreadUtil
{

struct Empty{/* Empty object for cases where we do not want to pass arguments to the thread */ };

template <typename T>
class Thread
{
private:

    std::thread thread;
    double freq;
    std::atomic<bool> running, paused; // Use std::atomic<bool> to prevent potential blocking between threads

    T* args;

    void thread_loop(void);

    std::function<void(T*)> thread_callback_handle;

public:
    // Use std::move to transfer ownership of the callback_handle to the Thread class. This removes potential lifetime issues
    Thread(const double&, std::function<void(T*)>);
    Thread(const double&, std::function<void(T*)>, T*);

    ~Thread();

    void start(void);
    void pause(void);
    void join(void);

};

#include "thread_util.hpp"

}

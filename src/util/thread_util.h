#pragma once

#include <atomic>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>

namespace ThreadUtil
{

template <typename T, typename T_handle>
std::function<void(T*)> bind(T_handle func_handle)
{
    return std::bind(&func_handle, std::placeholders::_1);
}

struct Empty { /* Empty object for cases where we do not want to pass arguments to the thread */ };

// A struct that can be interhited by args to be used as a mutex
struct ThreadMtx
{
    private:
        std::mutex mtx;
    
    public:
        void lock(void)
        {
            mtx.lock();
        }
        void unlock(void)
        {
            mtx.unlock();
        }
};

template <typename T>
struct ThreadQueue
{
    private:
        std::queue<T> queue;
        std::mutex queue_mtx;

    public: 
        void push(const T& data)
        {
            queue_mtx.lock();
            queue.push(data);
            queue_mtx.unlock();
        }
        T pop(void)
        {
            queue_mtx.lock();
            T data = queue.front();
            queue.pop();
            queue_mtx.unlock();
            return data;
        }
        T front(void)
        {
            queue_mtx.lock();
            T data = queue.front();
            queue_mtx.unlock();
            return data;
        }
        void emplace(const T& data)
        {
            queue_mtx.lock();
            queue.emplace(data);
            queue_mtx.unlock();
        }
        size_t size(void)
        {
            queue_mtx.lock();
            size_t queue_size = queue.size();
            queue_mtx.unlock();
            return queue_size;
        }
        bool empty(void)
        {
            queue_mtx.lock();
            bool is_empty = queue.empty();
            queue_mtx.unlock();
            return is_empty;
        }
};

template <typename T>
class Thread
{
private:

    std::thread thread;
    double freq;
    std::atomic<bool> running, paused; // Use std::atomic<bool> to prevent potential blocking between threads

    T* args;

    void thread_loop(void);
    void thread_constructor_loop(void);

    std::function<void(T*)> thread_callback_handle, thread_constructor_handle, thread_destructor_handle;

public:
    // Use std::move to transfer ownership of the callback_handle to the Thread class. This removes potential lifetime issues
    Thread(const double&, std::function<void(T*)>);
    Thread(const double&, std::function<void(T*)>, T*);
    Thread(const double&, std::function<void(T*)>, T*, std::function<void(T*)>, std::function<void(T*)>);

    ~Thread();

    void start(void);
    void pause(void);
    void join(void);

};

#include "thread_util.hpp"

} // namespace

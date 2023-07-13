
template <typename T>
ThreadUtil::Thread<T>::Thread(const double& freq, std::function<void(T*)> callback_handle)
    : freq(freq), running(true), paused(false), thread_callback_handle(std::move(callback_handle))
{
    thread = std::thread(&Thread::thread_loop, this);
    args = nullptr;
}

template <typename T>
ThreadUtil::Thread<T>::Thread(const double& freq, std::function<void(T*)> callback_handle, T* args)
    : freq(freq), running(true), paused(false), args(args), thread_callback_handle(std::move(callback_handle))
{
    thread = std::thread(&Thread::thread_loop, this);
}

template <typename T>
ThreadUtil::Thread<T>::Thread(const double& freq, std::function<void(T*)> callback_handle, T* args, 
    std::function<void(T*)> constructor_handle, std::function<void(T*)> destructor_handle)
    : freq(freq), running(true), paused(false), args(args), thread_callback_handle(std::move(callback_handle))
    , thread_constructor_handle(std::move(constructor_handle)), thread_destructor_handle(std::move(destructor_handle))
{
    thread = std::thread(&Thread::thread_constructor_loop, this);
}

template <typename T>
Thread<T>::~Thread()
{
    join();
}

template <typename T>
void ThreadUtil::Thread<T>::join(void)
{
    running = false;

    if (thread.joinable())
    {
        thread.join();
    }
}

template <typename T>
void ThreadUtil::Thread<T>::pause(void)
{
    paused = true;
}

template <typename T>
void ThreadUtil::Thread<T>::start(void)
{
    paused = false;
}

template <typename T>
void ThreadUtil::Thread<T>::thread_loop(void)
{
    while (running)
    {
        if (!paused)
        {
            thread_callback_handle(args);
           
            std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * (1.0 / freq))));
        }
        
    }
    
}

template <typename T>
void ThreadUtil::Thread<T>::thread_constructor_loop(void)
{
    thread_constructor_handle(args);
    while (running)
    {
        if (!paused)
        {
            thread_callback_handle(args);
           
            std::this_thread::sleep_for(std::chrono::milliseconds(int(1000 * (1.0 / freq))));
        }
        
    }
    thread_destructor_handle(args);
}


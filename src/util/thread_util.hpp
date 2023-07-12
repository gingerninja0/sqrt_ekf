
template <typename T>
Thread<T>::Thread(const double& freq, std::function<void(T*)> callback_handle)
    : freq(freq), running(true), paused(false), thread_callback_handle(std::move(callback_handle))
{
    thread = std::thread(&Thread::thread_loop, this);
    args = nullptr;
}

template <typename T>
Thread<T>::Thread(const double& freq, std::function<void(T*)> callback_handle, T* args)
    : freq(freq), running(true), paused(false), args(args), thread_callback_handle(std::move(callback_handle))
{
    thread = std::thread(&Thread::thread_loop, this);
}

template <typename T>
Thread<T>::~Thread()
{
    join();
}

template <typename T>
void Thread<T>::join(void)
{
    running = false;

    if (thread.joinable())
    {
        thread.join();
    }
}

template <typename T>
void Thread<T>::pause(void)
{
    paused = true;
}

template <typename T>
void Thread<T>::start(void)
{
    paused = false;
}

template <typename T>
void Thread<T>::thread_loop(void)
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


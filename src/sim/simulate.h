#pragma once

#include "../util/thread_util.h"

struct ThreadArgsTest : public ThreadUtil::ThreadMtx
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

template <typename T>
struct ThreadQueueTest : public ThreadUtil::ThreadQueue<T>
{
    private:
        int counter_1;
        int counter_2;
    public:
        void increment(void)
        {
            counter_1++;
        }
        void reset(void)
        {
            counter_1 = 0;
        }
        int get_counter_1(void) const
        {
            return counter_1;
        }
        void set_counter_1(const int val)
        {
            counter_1 = val;
        }
        int get_counter_2(void) const
        {
            return counter_2;
        }
        void set_counter_2(const int val)
        {
            counter_2 = val;
        }
};


void callback_empty_func(ThreadUtil::Empty*);

void callback_args_func(ThreadArgsTest*);
void constructor(ThreadArgsTest*);
void destructor(ThreadArgsTest*);

void callback_queue_1_func(ThreadQueueTest<int>*);
void callback_queue_2_func(ThreadQueueTest<int>*);
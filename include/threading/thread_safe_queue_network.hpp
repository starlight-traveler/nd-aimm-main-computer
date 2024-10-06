#ifndef THREAD_SAFE_QUEUE_NETWORK_H
#define THREAD_SAFE_QUEUE_NETWORK_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <utility>

template <typename T>
class ThreadSafeQueueNetwork
{
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable cond_var;

public:
    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(value));
        cond_var.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cond_var.wait(lock, [this]
                      { return !queue.empty(); });
        T value = std::move(queue.front());
        queue.pop();
        return value;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};

#endif // THREAD_SAFE_QUEUE_NETWORK_Hs

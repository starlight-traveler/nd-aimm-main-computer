#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
private:
    mutable std::mutex mtx;
    std::queue<T> dataQueue;
    std::condition_variable dataCond;

public:
    ThreadSafeQueue() {}

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mtx);
        dataQueue.push(std::move(new_value));
        dataCond.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lk(mtx);
        dataCond.wait(lk, [this]
                      { return !dataQueue.empty(); });
        T value = std::move(dataQueue.front());
        dataQueue.pop();
        return value;
    }

    bool tryPop(T &value)
    {
        std::lock_guard<std::mutex> lk(mtx);
        if (dataQueue.empty())
            return false;
        value = std::move(dataQueue.front());
        dataQueue.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mtx);
        return dataQueue.empty();
    }
};

#endif // THREAD_SAFE_QUEUE_H

#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <sstream>

// Thread-safe queue to store log messages
class LogQueue
{
public:
    void push(const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(message);
        cv.notify_one();
    }

    bool pop(std::string &result)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]
                { return !queue.empty(); });
        if (!queue.empty())
        {
            result = queue.front();
            queue.pop();
            return true;
        }
        return false;
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::string> queue;
};

extern LogQueue logQueue;

#endif // CONSOLE_LOGGER_H

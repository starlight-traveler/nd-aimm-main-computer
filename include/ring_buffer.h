#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <vector>
#include <string>

class RingBuffer
{
public:
    explicit RingBuffer(size_t capacity)
        : buffer_(capacity), head_(0), tail_(0), full_(false) {}

    void push(const std::string &value)
    {
        buffer_[head_] = value;
        head_ = (head_ + 1) % buffer_.size();
        if (full_)
        {
            tail_ = (tail_ + 1) % buffer_.size();
        }
        full_ = head_ == tail_;
    }

    std::string pop()
    {
        if (isEmpty())
        {
            return ""; // Return empty string if the buffer is empty
        }
        auto val = buffer_[tail_];
        tail_ = (tail_ + 1) % buffer_.size();
        full_ = false;
        return val;
    }

    bool isEmpty() const
    {
        return (!full_ && head_ == tail_);
    }

    bool isFull() const
    {
        return full_;
    }

private:
    std::vector<std::string> buffer_;
    size_t head_, tail_;
    bool full_;
};

#endif // RING_BUFFER_H

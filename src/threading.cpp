#include "threading.h"

/**
 * @brief Function to increment and log values in a loop
 */
void log_increment(std::atomic<int> &x, quill::Logger *logger)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        int val = ++x;
        LOG_INFO(logger, "Number: {}", val);
    }
}

void run_with_retry(quill::Logger *logger, ThreadSafeQueue<cv::Mat> &displayQueue)
{
    while (true)
    {
        try
        {
            orchestrationThreadLRCamera(logger, displayQueue);
            break; // Exit loop if setup_pipeline succeeds
        }
        catch (...)
        {
            // LOG_ERROR(logger, "Caught runtime error: {}", e.what());
            LOG_INFO(logger, "Retrying in 5 seconds...");
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
}
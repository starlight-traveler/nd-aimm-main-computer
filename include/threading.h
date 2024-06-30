#include "logger.h"
#include "pipeline.h"
#include "thread_safe_queue.h"

void log_increment(std::atomic<int> &x, quill::Logger *logger);
void run_with_retry(quill::Logger *logger, ThreadSafeQueue<cv::Mat> &displayQueue);
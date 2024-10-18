#ifndef THREAD_POOL_TESTING_H
#define THREAD_POOL_TESTING_H

#include "logger.h"

#include <map>
#include <string>


#include "BS_thread_pool.hpp" // BS::thread_pool
#include <future>             // std::future
#include <iostream>           // std::cout

namespace testing_threadpool
{
    bool threadpool_test(quill::Logger *logger);
}
#endif // PERIPHERAL_TESTING_H
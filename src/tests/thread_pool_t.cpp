#include "thread_pool_t.h"

namespace testing_threadpool {


    double multiply(const double lhs, const double rhs)
    {
        return lhs * rhs;
    }

    bool threadpool_test(quill::Logger *logger) {

        BS::thread_pool pool;

        constexpr double first = 6;
        constexpr double second = 7;

        std::future<double> my_future = pool.submit_task(
            [first, second]
            {
                return multiply(first, second);
            });

            LOG_DEBUG(logger, "Thread pool task verified, {}", my_future.get());

            return true;

    }




}

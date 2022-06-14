#ifndef IPC_H
#define IPC_H

#include "equation.h"
#include "task.h"

#include <atomic>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

constexpr unsigned long MIN_DEFAULT_THREADS_NUM {3};
constexpr unsigned int LEAST_MAX_SEMAPHORE_VALUE {32};

template <typename T>
struct ipc_data_t {
    const unsigned int consumer_threads_number;
    std::vector<T> buffer;
    std::counting_semaphore<LEAST_MAX_SEMAPHORE_VALUE> number_of_empty_elements;
    std::counting_semaphore<LEAST_MAX_SEMAPHORE_VALUE> number_of_queueing_elements;
    std::mutex buffer_mutex;
    std::atomic<bool> producer_finished {false};

    ipc_data_t(const unsigned int consumer_threads_number)
        : consumer_threads_number(consumer_threads_number),
          buffer({}),
          number_of_empty_elements {consumer_threads_number},
          number_of_queueing_elements {0},
          buffer_mutex()
    {
        buffer.reserve(consumer_threads_number);
    }
};

unsigned long const get_threads_number()
{
    unsigned long const hardware_threads_num = std::thread::hardware_concurrency();
    unsigned long const min_threads_num = MIN_DEFAULT_THREADS_NUM;
    return hardware_threads_num ? hardware_threads_num : min_threads_num;
}

#endif // IPC_H

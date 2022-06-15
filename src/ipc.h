#ifndef IPC_H
#define IPC_H

#include "equation.h"
#include "task.h"

#include <atomic>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

//@PURPOSE: Provide an IPC shared data struct realization for equation-solver app
//
//@CLASSES:
//  ipc_data_t: helper struct, container for all producer-consumer pattern operations
//
//@DESCRIPTION: producer-consumer struct container holds:
// - buffer: std::vector that will hold shared data
// - first semaphore is counting amount of empty elements. It is needed so that
//   the producer can block when there are no more empty slots in the buffer.
// - second semaphore is counting amount of queing(processing) elements. It is
//   needed so that the consumer can block when there are no elements are ready
//   to be processed.
// - mutex for exclusive buffer access
// - atomic bool variable that is used as a flag for consumer threads to halt end
//   their lifetime

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
        // Here exact maximum amount of needed elements reserved to avoid unnecessary
        // reallocations in future.
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

#ifndef IPC_H
#define IPC_H

#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

constexpr unsigned long MIN_DEFAULT_THREADS_NUM {2};
constexpr unsigned int LEAST_MAX_SEMAPHORE_VALUE {32};

template <typename T, std::ptrdiff_t U>
struct ipc_data_t {
    std::vector<T> m_buffer;
    std::counting_semaphore<U> m_number_of_empty_elements;
    std::counting_semaphore<U> m_number_of_queueing_elements;
    std::mutex m_buffer_mutex;

    ipc_data_t(const unsigned int max_elements_number)
        : m_buffer({max_elements_number}),
          m_number_of_empty_elements {max_elements_number},
          m_number_of_queueing_elements {0},
          m_buffer_mutex()
    {}
};

unsigned long const get_threads_number()
{
    unsigned long const hardware_threads_num = std::thread::hardware_concurrency();
    unsigned long const min_threads_num = MIN_DEFAULT_THREADS_NUM;
    return hardware_threads_num ? hardware_threads_num : min_threads_num;
}

#endif // IPC_H

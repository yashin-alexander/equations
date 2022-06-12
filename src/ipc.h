#ifndef IPC_H
#define IPC_H

#include <mutex>
#include <vector>
#include <thread>
#include <semaphore>

constexpr int MIN_DEFAULT_THREADS_NUM{2};
constexpr int LEAST_MAX_SEMAPHORE_VALUE{32};

template <typename T, std::ptrdiff_t U>
struct shared_parameters_t {
    std::vector<T>& m_buffer;
    std::counting_semaphore<U>& m_number_of_empty_elements;
    std::counting_semaphore<U>& m_number_of_queueing_elements;
    std::mutex& m_buffer_mutex;
};

template <typename T, std::ptrdiff_t U>
class Producer {
private:
    shared_parameters_t<T, U> m_ipc_ctl;
public:
    Producer(shared_parameters_t<T, U> ipc_cfg);
};

template <typename T, std::ptrdiff_t U>
Producer<T, U>::Producer(shared_parameters_t<T, U> ipc_cfg) : m_ipc_ctl(ipc_cfg) {};

template <typename T, std::ptrdiff_t U>
class Consumer {
private:
    shared_parameters_t<T, U> m_ipc_ctl;
public:
    Consumer(shared_parameters_t<T, U> ipc_cfg);
};

template <typename T, std::ptrdiff_t U>
Consumer<T, U>::Consumer(shared_parameters_t<T, U> ipc_cfg) : m_ipc_ctl(ipc_cfg) {};


unsigned long const get_threads_number() {
    unsigned long const hardware_threads_num = std::thread::hardware_concurrency();
    unsigned long const min_threads_num = MIN_DEFAULT_THREADS_NUM;
    return hardware_threads_num ? hardware_threads_num : min_threads_num;
}


#endif // IPC_H

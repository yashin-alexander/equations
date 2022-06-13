#ifndef PRODUCER_H
#define PRODUCER_H

#include "ipc.h"
#include "task.h"

#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

/*
 * This code implements Dijkstra's solution of producer-consumer problem with
 * two semaphores for signaling purposes and mutex for buffer exclusive
 * access operations.
 *
 * Also, I've added graceful threads halt check. After finishing it's job
 * Producer will set up atomic variable and then will signalize semaphore
 * without actual data generation. Consumers will find it out and exit.
 */

template <typename T>
class Producer
{
private:
    ipc_data_t<T> &m_ipc_data;
    void halt();

public:
    Producer(ipc_data_t<T> &ipc_cfg) : m_ipc_data(ipc_cfg) {};
    void run(const std::vector<QuadraticTask> &tasks_source);
};

template <typename T>
void Producer<T>::halt()
{
    m_ipc_data.producer_finished.store(true);
    for (int i = 0; i < m_ipc_data.consumer_threads_number; i++) {
        m_ipc_data.number_of_empty_elements.acquire();
        m_ipc_data.number_of_queueing_elements.release();
    }
}

template <typename T>
void Producer<T>::run(const std::vector<QuadraticTask> &tasks_source)
{
    for (const auto &t : tasks_source) {
        m_ipc_data.number_of_empty_elements.acquire();
        {
            std::lock_guard g(m_ipc_data.buffer_mutex);
            m_ipc_data.buffer.emplace_back(t);
        }
        m_ipc_data.number_of_queueing_elements.release();
    }
    halt();
}

#endif // PRODUCER_H

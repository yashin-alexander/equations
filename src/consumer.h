#ifndef CONSUMER_H
#define CONSUMER_H

#include "ipc.h"
#include "task.h"

#include <iostream>
#include <mutex>
#include <semaphore>
#include <sstream>
#include <thread>

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
class Consumer
{
private:
    ipc_data_t<T> &m_ipc_data;
    void processTask(const QuadraticTask &task);

public:
    Consumer(ipc_data_t<T> &ipc_cfg) : m_ipc_data(ipc_cfg) {};
    void run();
};

template <typename T>
void Consumer<T>::processTask(const QuadraticTask &task)
{
    QuadraticEquation equation {task};
    equation.solve();
    const auto results = equation.get_task_results();
    std::stringstream ss;
    ss << "(" << task.a << "*x*x)+(" << task.b << "*x)+" << task.c << " x=" << results.root1
       << " x=" << results.root2 << " extremum=" << results.extremum << '\n';
    std::cout << ss.str();
}

template <typename T>
void Consumer<T>::run()
{
    for (;;) {
        m_ipc_data.number_of_queueing_elements.acquire();
        QuadraticTask task {};
        {
            std::lock_guard g(m_ipc_data.buffer_mutex);
            if (m_ipc_data.buffer.empty() and m_ipc_data.producer_finished.load()) {
                return;
            }
            task = m_ipc_data.buffer.back();
            m_ipc_data.buffer.pop_back();
        }
        m_ipc_data.number_of_empty_elements.release();
        processTask(task);
    }
}

#endif // CONSUMER_H

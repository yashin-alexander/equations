#ifndef PRODUCER_H
#define PRODUCER_H

#include "argparse.h"
#include "ipc.h"
#include "task.h"

#include <iostream>
#include <mutex>
#include <optional>
#include <semaphore>
#include <set>
#include <sstream>

//@PURPOSE: Provide a producer realization for equation-solver app
//
//@CLASSES:
//  Producer: capable of creating new tasks and sending them for execution
//
//@DESCRIPTION: This code implements Dijkstra's solution of producer-consumer
// problem with two semaphores for signaling purposes and mutex for buffer
// exclusive access operations.
//
// Threads halt logic is also added: After finishing it's job
// Producer will set up atomic variable and then will signalize semaphore
// without actual data generation. Consumers will find it out and exit.

template <typename T>
class Producer
{
private:
    ipc_data_t<T> &m_ipc_data;
    std::set<T> solved_tasks;
    bool taskIsAlreadySolved(T &task);
    void halt();

public:
    Producer(ipc_data_t<T> &ipc_cfg) : m_ipc_data(ipc_cfg) {};
    void run(ArgumentParser &argparse);
};

template <typename T>
bool Producer<T>::taskIsAlreadySolved(T &task)
{
    // We have a cache with solved tasks here to not to repeat the job
    if (solved_tasks.find(task) != solved_tasks.end()) {
        std::stringstream ss;
        ss << "(" << task.a << "*x*x)+(" << task.b << "*x)+" << task.c
           << " has already been given, skipping unnecessary recalculation\n";
        std::cout << ss.str();
        return true;
    }
    solved_tasks.insert(task);
    return false;
}

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
void Producer<T>::run(ArgumentParser &argparse)
{
    for (auto task_opt = argparse.parseNext(); task_opt != std::nullopt;
         task_opt = argparse.parseNext()) {
        auto task = task_opt.value();
        if (taskIsAlreadySolved(task)) {
            continue;
        }
        m_ipc_data.number_of_empty_elements.acquire();
        {
            std::lock_guard g(m_ipc_data.buffer_mutex);
            m_ipc_data.buffer.emplace_back(task);
        }
        m_ipc_data.number_of_queueing_elements.release();
    }
    halt();
}

#endif // PRODUCER_H

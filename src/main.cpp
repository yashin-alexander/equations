#include "argparse.h"
#include "equation.h"
#include "ipc.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

void producer(std::vector<QuadraticTask> &tasks_source,
              ipc_data_t<QuadraticTask, LEAST_MAX_SEMAPHORE_VALUE> &ipc_data)
{
    for (const auto &t : tasks_source) {
        ipc_data.m_number_of_empty_elements.acquire();
        {
            std::lock_guard g(ipc_data.m_buffer_mutex);
            ipc_data.m_buffer.emplace_back(t);
        }
        ipc_data.m_number_of_queueing_elements.release();
    }
}

void processTask(const QuadraticTask &task)
{
    QuadraticEquation equation {task};
    equation.solve();
    const auto results = equation.get_task_results();
    std::stringstream ss;
    ss << "(" << task.a << "*x*x)+(" << task.b << "*x)+" << task.c << " x=" << results.root1
       << " x=" << results.root2 << " extremum: " << results.extremum << '\n';
    std::cout << ss.str();
}

void consumer(ipc_data_t<QuadraticTask, LEAST_MAX_SEMAPHORE_VALUE> &ipc_data)
{
    for (;;) {
        ipc_data.m_number_of_queueing_elements.acquire();
        QuadraticTask task {};
        {
            std::lock_guard g(ipc_data.m_buffer_mutex);
            task = ipc_data.m_buffer.back();
            ipc_data.m_buffer.pop_back();
        }
        ipc_data.m_number_of_empty_elements.release();
        processTask(task);
    }
}

int main(int argc, char **argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    std::vector<QuadraticTask> tasks {};
    ArgumentParser parser {arguments};
    if (!parser.parse(&tasks)) {
        return -1;
    }

    const unsigned int threads_number = get_threads_number() -
                                        2; // we have 1 main thread and 1 producer
    std::cout << "Threads number: " << threads_number << '\n';
    ipc_data_t<QuadraticTask, LEAST_MAX_SEMAPHORE_VALUE> ipc_data {threads_number};

    std::thread p(producer, std::ref(tasks), std::ref(ipc_data));
    for (int i = 0; i < threads_number; i++) {
        std::thread c(consumer, std::ref(ipc_data));
        c.detach();
    }
    p.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // let daemons end their work
    return 0;
}

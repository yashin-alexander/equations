#include "argparse.h"
#include "equation.h"
#include "ipc.h"

#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <sstream>
#include <semaphore>


//https://www.codeproject.com/Articles/25294/Avoiding-Overflow-Underflow-and-Loss-of-Precision

void producer(std::vector<QuadraticTask>& tasks,
              shared_parameters_t<QuadraticTask, LEAST_MAX_SEMAPHORE_VALUE>& ipc_data) {
    for (const auto& t: tasks) {
        ipc_data.m_number_of_empty_elements.acquire();
        {
            std::lock_guard g(ipc_data.m_buffer_mutex);
            ipc_data.m_buffer.emplace_back(t);
        }
        ipc_data.m_number_of_queueing_elements.release();
    }
    std::cout << "Teardown producer! \n";
}

void consumer(shared_parameters_t<QuadraticTask, LEAST_MAX_SEMAPHORE_VALUE>& ipc_data) {
    for (;;) {
        ipc_data.m_number_of_queueing_elements.acquire();
        QuadraticTask task;
        {
            std::lock_guard g(ipc_data.m_buffer_mutex);
            task = ipc_data.m_buffer.back();
            ipc_data.m_buffer.pop_back();
        }
        ipc_data.m_number_of_empty_elements.release();
        // Processing calculations
        QuadraticEquation equation{task};
        equation.solve();
        if (equation.is_solved()) {
            const QuadraticTaskResults results = equation.get_task_results();
            std::stringstream ss;
            ss << "Worker: " << std::this_thread::get_id() << " "
               << task.a << "*x*x+" << task.b << "*x+" << task.c
               << " roots: (" << results.root1
               << ", " << results.root2
               << "); extremum: " << results.extremum << '\n';
            std::cout << ss.str();
        }
    }
}


int main(int argc, char **argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    std::vector<QuadraticTask> tasks{};
    std::vector<QuadraticTask> ipc_buffer{};
    ArgumentParser parser{arguments};
    if (!parser.parse(&tasks)) {
        return -1;
    }

    const int threads_number = get_threads_number() - 1;  // dec main thread
    std::cout << "Threads number: " << threads_number << '\n';
    ipc_buffer.reserve(threads_number);

    std::counting_semaphore<LEAST_MAX_SEMAPHORE_VALUE> number_of_queueing_portions{0};
    std::counting_semaphore<LEAST_MAX_SEMAPHORE_VALUE> number_of_empty_positions{threads_number};
    std::mutex buffer_manipulation;

    shared_parameters_t<QuadraticTask, LEAST_MAX_SEMAPHORE_VALUE> ipc_data {
        ipc_buffer,
        number_of_empty_positions,
        number_of_queueing_portions,
        buffer_manipulation
    };

    std::thread p(producer, std::ref(tasks), std::ref(ipc_data));
    std::thread c1(consumer, std::ref(ipc_data));
    std::thread c2(consumer, std::ref(ipc_data));
    std::thread c3(consumer, std::ref(ipc_data));
    p.join();
    c1.join();
    c2.join();
    c3.join();
    return 0;
}

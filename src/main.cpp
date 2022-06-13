#include "argparse.h"
#include "consumer.h"
#include "equation.h"
#include "ipc.h"
#include "producer.h"

#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char **argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    std::vector<QuadraticTask> tasks {};
    ArgumentParser parser {arguments};
    if (!parser.parse(&tasks)) {
        return -1;
    }

    // we have 1 main thread and 1 producer
    const unsigned int threads_number = get_threads_number() - 2;
    std::cout << "Threads number: " << threads_number << '\n';
    ipc_data_t<QuadraticTask> ipc_data {threads_number};

    Producer producer {ipc_data};
    std::thread p(&Producer<QuadraticTask>::run, &producer, std::ref(tasks));
    p.detach();

    Consumer c {ipc_data};
    std::vector<std::thread> consumers {};
    for (int i = 0; i < threads_number; i++) {
        consumers.emplace_back(&Consumer<QuadraticTask>::run, &c);
    }
    for (auto &c : consumers) {
        c.join();
    }
    return 0;
}

#include "argparse.h"
#include "equation.h"

#include <iostream>
#include <vector>
#include <cstdint>


int main(int argc, char **argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    ArgumentParser parser{arguments};
    if (!parser.arguments_are_valid()) {
        std::cout << "Input parameters are invalid!" << std::endl;
        return 1;
    }
    for (const auto & s: arguments){
        std::cout << s << '\n';
    }
    return 0;
}

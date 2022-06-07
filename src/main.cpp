#include "argparse.h"
#include "equation.h"

#include <iostream>
#include <vector>


int main(int argc, char **argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    ArgumentParser parser{arguments};

    for (const auto & s: arguments){
        std::cout << s << '\n';
    }
    std::cout << "valid: " << parser.is_valid() << std::endl;
    return 0;
}

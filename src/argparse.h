#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <vector>
#include <string>

class ArgumentParser {
public:
    ArgumentParser(std::vector<std::string>& arguments) : arguments(arguments) {};
    bool is_valid();
private:
    std::vector<std::string>& arguments;
};

#endif // ARGPARSE_H

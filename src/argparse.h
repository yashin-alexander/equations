#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "task.h"

#include <vector>
#include <string>

class ArgumentParser {
public:
    explicit ArgumentParser(const std::vector<std::string>& arguments) : m_arguments(arguments) {};
    bool parse(std::vector<TaskSquared>& tasks) const;
private:
    const std::vector<std::string>& m_arguments;
    static bool string_to_int(const std::string& src_string, int& dst_int);
    bool arguments_are_valid() const;
    bool arguments_number_is_valid() const;
    bool arguments_types_are_valid() const;
};

#endif // ARGPARSE_H

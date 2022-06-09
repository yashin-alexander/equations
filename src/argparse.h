#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "task.h"

#include <vector>
#include <string>
#include <iterator>

using ArgumentsIterator = std::vector<std::string>::const_iterator;

class ArgumentParser {
public:
    explicit ArgumentParser(const std::vector<std::string>& arguments) : m_arguments(arguments) {};
    bool parse(std::vector<QuadraticTask>* tasks) const;
private:
    const std::vector<std::string>& m_arguments;
    static bool task_from_three_strings(QuadraticTask* task, const ArgumentsIterator& iterator);
    static bool int_from_string(int* dst_int, const std::string& src_string);
    bool arguments_are_valid() const;
    bool arguments_number_is_valid() const;
    bool arguments_types_are_valid() const;
};

#endif // ARGPARSE_H

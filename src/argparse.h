#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "task.h"

#include <optional>
#include <string>
#include <vector>

using ArgumentsIterator = std::vector<std::string>::const_iterator;

class ArgumentParser
{
public:
    explicit ArgumentParser(const std::vector<std::string> &arguments)
        : m_arguments(arguments), m_argumets_iter(m_arguments.begin()) {};
    std::optional<QuadraticTask> parseNext();
    bool argumentsNumberIsValid() const;

private:
    const std::vector<std::string> &m_arguments;
    std::vector<std::string>::const_iterator m_argumets_iter;
    static bool taskFromThreeArguments(QuadraticTask *task, const ArgumentsIterator &iterator);
    static bool intFromString(int *dst_int, const std::string &src_string);
    bool threeArgumentsAreValid(const ArgumentsIterator &iterator) const;
    bool argumentTypeIsValid(const std::string &candidate_str) const;
};

#endif // ARGPARSE_H

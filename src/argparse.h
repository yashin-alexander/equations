#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "task.h"

#include <optional>
#include <string>
#include <vector>

//@PURPOSE: Provide a arguments parser for equation-solver app
//
//@CLASSES:
//  ArgumentParser: arguments parser for equations solver app
//
//@DESCRIPTION: This component provides ariguments parser that
// is able to parse triplets of int type arguments.
// All of the arguments must be less than 9 digits long.
// Class provides 'parseNext()' method that allows to
// parse next portion of arguments data.
// Also this class is capable of initial testing for an arguments
// number with the 'argumentsNumberIsValid()' method

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

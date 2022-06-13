#include "argparse.h"

#include <iostream>
#include <regex>

namespace {
constexpr int QADRATIC_EQUATION_PARAMETERS_NUMBER {3};
constexpr const char *SIGNED_INT_REGEX_PATTERN {"[-]?[0-9]{1,9}"};
} // namespace

bool ArgumentParser::parse(std::vector<QuadraticTask> *tasks) const
{
    if (!arguments_are_valid()) {
        std::cout << "Input parameters are invalid!" << std::endl;
        return false;
    }
    QuadraticTask task {};
    for (auto it = m_arguments.begin(), ite = m_arguments.end(); it != ite;
         it += QADRATIC_EQUATION_PARAMETERS_NUMBER) {
        if (task_from_three_strings(&task, it)) {
            tasks->emplace_back(task);
        } else {
            std::cout << "ArgumentParser: unable to process arguments \n";
            return false;
        }
    }
    return true;
}

bool ArgumentParser::task_from_three_strings(QuadraticTask *task, const ArgumentsIterator &iterator)
{
    return (ArgumentParser::int_from_string(&task->a, *iterator) &&
            ArgumentParser::int_from_string(&task->b, *(iterator + 1)) &&
            ArgumentParser::int_from_string(&task->c, *(iterator + 2)));
}

bool ArgumentParser::int_from_string(int *dst_int, const std::string &src_string)
{
    try {
        *dst_int = std::stoi(src_string);
    } catch (std::invalid_argument const &ex) {
        std::cout << "ArgumentParser: unable to convert string to int: " << src_string << '\n';
        return false;
    } catch (std::out_of_range const &ex) {
        std::cout << "ArgumentParser: int value is out of range: " << src_string << '\n';
        return false;
    }
    return true;
}

bool ArgumentParser::arguments_are_valid() const
{
    return arguments_number_is_valid() && arguments_types_are_valid();
}

bool ArgumentParser::arguments_types_are_valid() const
{
    const std::regex signed_int_regex {SIGNED_INT_REGEX_PATTERN};

    for (const auto &s : m_arguments) {
        if (!std::regex_match(s, signed_int_regex)) {
            std::cout << "ArgumentParser: parameter with incorrect type provided: {" << s << "}"
                      << ", please, follow the regex: " << SIGNED_INT_REGEX_PATTERN << std::endl;
            return false;
        }
    }
    return true;
}

bool ArgumentParser::arguments_number_is_valid() const
{
    const auto args_number = m_arguments.size();
    if ((args_number == 0) or (args_number % QADRATIC_EQUATION_PARAMETERS_NUMBER != 0)) {
        std::cout << "ArgumentParser: incorrect number of arguments provided: {" << args_number
                  << "}" << std::endl;
        return false;
    }
    return true;
}


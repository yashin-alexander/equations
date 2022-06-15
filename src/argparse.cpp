#include "argparse.h"

#include <iostream>
#include <regex>

namespace {
constexpr int QADRATIC_EQUATION_PARAMETERS_NUMBER {3};
constexpr const char *SIGNED_INT_REGEX_PATTERN {"[-]?[0-9]{1,9}"};
} // namespace

std::optional<QuadraticTask> ArgumentParser::parseNext()
{
    while (m_argumets_iter < m_arguments.end()) {
        QuadraticTask task {};
        if (threeArgumentsAreValid(m_argumets_iter) &&
            taskFromThreeArguments(&task, m_argumets_iter)) {
            m_argumets_iter += QADRATIC_EQUATION_PARAMETERS_NUMBER;
            return {task};
        }
        std::cout << "ArgumentParser: dropping arguments pack: {" << *m_argumets_iter << " "
                  << *(m_argumets_iter + 1) << " " << *(m_argumets_iter + 2) << "} \n";
        m_argumets_iter += QADRATIC_EQUATION_PARAMETERS_NUMBER;
    }
    return {std::nullopt};
}

bool ArgumentParser::taskFromThreeArguments(QuadraticTask *task, const ArgumentsIterator &iterator)
{
    return (ArgumentParser::intFromString(&task->a, *iterator) &&
            ArgumentParser::intFromString(&task->b, *(iterator + 1)) &&
            ArgumentParser::intFromString(&task->c, *(iterator + 2)));
}

bool ArgumentParser::intFromString(int *dst_int, const std::string &src_string)
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

bool ArgumentParser::threeArgumentsAreValid(const ArgumentsIterator &iterator) const
{
    return (ArgumentParser::argumentTypeIsValid(*iterator) &&
            ArgumentParser::argumentTypeIsValid(*(iterator + 1)) &&
            ArgumentParser::argumentTypeIsValid(*(iterator + 2)));
}

bool ArgumentParser::argumentTypeIsValid(const std::string &candidate_str) const
{
    static const std::regex signed_int_regex {SIGNED_INT_REGEX_PATTERN};

    if (!std::regex_match(candidate_str, signed_int_regex)) {
        std::cout << "ArgumentParser: parameter with incorrect type provided: {" << candidate_str
                  << "}"
                  << ", please, follow the regex: " << SIGNED_INT_REGEX_PATTERN << std::endl;
        return false;
    }
    return true;
}

bool ArgumentParser::argumentsNumberIsValid() const
{
    const auto args_number = m_arguments.size();
    if ((args_number == 0) or (args_number % QADRATIC_EQUATION_PARAMETERS_NUMBER != 0)) {
        std::cout << "ArgumentParser: incorrect number of arguments provided: {" << args_number
                  << "}" << std::endl;
        return false;
    }
    return true;
}

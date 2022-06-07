#include "argparse.h"

#include <algorithm>

bool ArgumentParser::is_valid() {
    /*
     *
     */
    for (const auto& s: arguments) {
         if (!std::all_of(s.begin(), s.end(), ::isdigit)){
             return false;
         }
    }
    return true;
}

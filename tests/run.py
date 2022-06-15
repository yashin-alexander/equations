#!/usr/bin/python3

import sys
import random
import subprocess

MIN_INT = -1000
MAX_INT = +1000
TESTING_EPSILON = 0.1


def generate_single_params_pack(min_int, max_int):
    a = random.randint(min_int, max_int)
    b = random.randint(min_int, max_int)
    c = random.randint(min_int, max_int)
    return " {} {} {}".format(a, b, c)

def check_single_root(root_value_str, equation_str):
    if "nan" in root_value_str:
        return ("UNTESTED: {}")
    exec(root_value_str)
    r = eval(equation_str)
    if (abs(r) < TESTING_EPSILON):
        return ("OK: {} delta: " + str(r))
    else:
        return ("WRONG: {} delta: " + str(r) + " https://www.wolframalpha.com/input?i=" + equation_str + "=0 ")

def parse_results(results, equations_number):
    str_results = results.decode("utf-8")
    workers_counter = 0
    for s in str_results.split("\n"):
        if "extremum" in s:
            words = s.split()
            root1_test_result = check_single_root(words[1], words[0])
            root2_test_result = check_single_root(words[2], words[0])
            print(root1_test_result.format(s))
            print(root2_test_result.format(s))
            workers_counter += 1

if (__name__ == "__main__"):
    equations_number = int(sys.argv[1])
    for multiplier in range(25):
        cmd = "./build/equations"
        for _ in range(equations_number):
            cmd += generate_single_params_pack(-(2**multiplier), 2**multiplier)
        print(cmd)
        output = subprocess.check_output(cmd, shell=True)
        parse_results(output, equations_number)

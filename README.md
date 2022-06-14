This code is assumed to be compiled with at least *gcc-11*, as some C++20 features were used here.

Install gcc-11 via apt, then build and use an app: 
```
make
./build/equations 1 -2 -3 0 4 -4 11 22 0
```
Solutions output order is undefined, but it's guaranteed that all solutions will be provided.
Huge integer calculations are not supported.

It's also possible to run functional testing with:
```
./tests/run.py 100 
```
This script will generate a set of random tasks and check whether results are correct.


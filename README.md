# py-cpp-io-perf

Compare the IO performance of C++, Python, and Pypy. 

If you want to write some IO intensive applications and are hesitating about
which language, C++ or Python,  to use. Here is a set of benchmarks that could
tell which one is faster and how much faster. 

To tell the differences of languages, we have written two benchmarks. One is 
in Python and another in C++. They access a file with the same set of patterns. 
Any performance difference should be attributed to the language. Since Pypy
can improve Python performance, we also can run the Python benchmark with pypy.


## To run Python benchmark:

  $ make pythonrun

## To run Python benchmark with Pypy:

Edit Makefile, change pythonpath to your pypy path. For example:

pythonpath=/users/jhe//pypy-5.1.1-linux64/bin/pypy

Then:

$ make pythonrun

## To run C++ benchmark:

$ make cpprun

## To run Python and cpp benchmarks together:

$ make run




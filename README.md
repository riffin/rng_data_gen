# rng_data_gen

Simple file generator with a file reader to HTTP "POST".

# Dependencies

* cURLpp
* pthread
* standart filesystem

# Execution

On random iteration of 0.5 to 15 seconds a file is generated with a random value and unix timestamp.
The files are read into a string and afterwards posted to HTTP server via "POST" every 60 seconds.

# Installation

To compile `make` is enough. Before `make` please install cURLpp.

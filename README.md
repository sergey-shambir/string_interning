string_interning
================

Simpliest example and benchmark for C++/Qt string interning.

It's implemented as command-line program with one parameter: path to directory with really big C++ project sources. Program takes all numbers and identifiers from each *.cpp and *.h, inserts them to string pool and measures some parameters. Can use either std or Qt data structures (see project file).

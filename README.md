_This project is a labwork #4 I've done in the 1st semester of ITMO University bachelor in software engineering, "C++
programming intro" course. The README text is the assignment as it was given._

# ArgParser

## Task

Design and implement a class for parsing command-line arguments.

## Requirements

You are provided with a [test file](tests/argparser_test.cpp) as the requirements. Based on these tests, you need to figure out the public
interface that the parser should provide. Your task is to ensure that all tests pass.

Initially, the parser only handles strings, integers, and boolean arguments. You are allowed to add other types 
and extend the functionality.

### Tests

Similar to previous lab assignments, [GoogleTest](https://google.github.io/googletest/) is used as the testing framework. 
The provided tests use only two types of assertions:
`ASSERT_TRUE`, `ASSERT_EQ`



## Implementation

The implementation should locate in the [argparser](lib/CMakeLists.txt) library. Initially, the library contains one
class; if necessary, add new files, classes, functions, etc., to the library.

### Example input

An example program using the parser is found in [bin](bin/main.cpp). The program can sum or multiply the arguments 
passed to it.

`labwork4 --sum 1 2 3 4 5`

`labwork4 --mult 1 2 3 4 5`

## NB

1. Completing this assignment assumes **basic** knowledge of classes. You are allowed to use templates, 
virtual functions, etc., but you need to understand how they work and be prepared for questions.

2. This lab assignment may be reused in subsequent labs as a library for handling command-line arguments.
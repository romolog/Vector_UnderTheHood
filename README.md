# Vector: implementation of std::vector


I implemented a container that is as close to std::vector as my current level of C++ proficiency allows.

This project is still ongoing — from time to time I add new code to implement additional features or improve previous solutions. I treat it as a kind of achievement board, where I incorporate and reinforce every new concept or technique I learn.

-------

I implemented a container that is a limited version of std::vector.

I keep this project ongoing and treat it as a kind of achievement board where I incorporate and reinforce every new concept or technique I learn. More features and improvements are yet to come.

-------

cmake -S . -B build
cmake --build build
ctest --test-dir build 				// concise output
ctest --test-dir build --verbose 	// keeps GTest format

ATTENTION: check MyVector_test.cpp lines 5-59 for more info
# Vector: implementation of std::vector


I implemented a container that provides a limited subset of std::vector functionality.

The project showcases my current skills in C++, including templates, allocators, iterators, and unit testing with Google Test.

Although I have written more than 100 tests, I still have many ideas and additional cases to add. However, my goal was to create a limited, academic implementation of std::vector, and at this point I consider it achieved. With some regret, I am stopping here and moving on to the next project.

-------

Implemented features:
<pre>
* Exception safety:			RAII storage ensures that destructors are called, even when exceptions occur.

* Iterators:				Custom iterators are implemented.

* Allocators:				MyVector is allocator-aware. Custom allocators are used in the tests.
					
* GTests:					More than 100 unit tests cover the implementation's behavior.

* No Leaks or errors:		No memory leaks or errors. Compiled with ASan, UBSan, and LSan sanitizers. 
							Corner cases verified by Valgrind.

* No AI-generated code:		All code is written manually, while being "C++"-conscious 
							and with caffeine levels high.

</pre>

---

### System Requirements

- Linux (Ubuntu)
- clang++
- cmake
- make
- bash

---

### Build & Run

```bash
git clone https://github.com/romolog/CPP__algorithm__ford-johnson_2025 my_vector;
cd my_vector;
./test.sh;

```



# Vector: implementation of std::vector


I implemented a container that provides a limited subset of std::vector functionality.

The project showcases my current skills in C++, including templates, allocators, iterators, and unit testing with Google Test. I plan to continue expanding the implementation with additional features, optimizations, and test coverage.

-------

Implemented features:
<pre>
* Exception safety:		RAII storage ensures destructor call for memory allocated objects

* Iterators:			Custom iterators are implemented.

* Allocators:			MyVector is projected as allocator awared.
					
* GTests:				A large part of implementation is covered by unit tests. More tests will be added.
</pre>

---

### System Requirements

- Linux (Ubuntu)
- g++/clang++
- cmake
- make

---

### Build

```bash
git clone https://github.com/romolog/CPP__algorithm__ford-johnson_2025 my_vector;
cd my_vector;
cmake -S . -B build;
cmake --build build

```

### Run
```bash
ctest --test-dir build;
```



// TEST STRUCTURE
//
// ├── includes
// ├── helper functions
// ├── helper classes
// ├── test fixtures
// ├── typed-test declarations
// ├── TEST(...)
// ├── TEST_F(...)
// ├── TYPED_TEST(...)


// | Feature              | Purpose                      |
// | -------------------- | ---------------------------- |
// | `TEST`               | single test                  |
// | `TEST_F`             | shared fixture               |
// | `TYPED_TEST`         | same test for multiple types |
// | `TYPED_TEST_P`       | extensible typed tests       |
// | `TYPED_TEST_SUITE_P` | reusable test patterns       |


//	TEST_F
//		TEST_F = TEST + shared fixture object
//		Each test gets its own fresh instance of the fixture.
//
// class VectorTest : public ::testing::Test
// {
// protected:
//     myvec::MyVector<int> v;

//     void SetUp() override
//     {
//         // runs before each test
//     }

//     void TearDown() override
//     {
//         // runs after each test
//     }
// };
//
//
// 1. Create VectorTest object
// 2. Run SetUp()
// 3. Run test body
// 4. Run TearDown()
// 5. Destroy object
//
//		USAGE:
//	TEST_F(VectorTest, Empty)
//	{
//		EXPECT_TRUE(v.empty());
//	}


// 0 -> no elements
// 1 -> first/last element are the same object
// N -> general case

// cmake -S . -B build
// cmake --build build
// ./build/tests 
//		OR
//	ctest --test-dir build --verbose
//		OR
//	ctest --test-dir build --overwrite MemoryCheckCommand=valgrind -T memcheck

//	Command			| Meaning
//	------------------------------------------------------------------
//	-S .			| source directory — where CMakeLists.txt is
//	-B build		| build directory — where to put generated files
//	--build build	| compile everything in build/

// # Configure
// cmake -S . -B build 
//     -DCMAKE_BUILD_TYPE=Release 
//     -DTESTS=ON 
//     -GNinja
//
// # Build
// cmake --build build --parallel
//
// # Run
// ./build/tests


// ctest
//   ↓
// reads test registry (added by gtest_discover_tests())
//   ↓
// launches ./build/tests for each test
//   ↓
// collects results
//   ↓
// reports pass/fail summary


// +------------------------+------------------+----------------------+
// | Feature                | ./build/tests    | ctest                |
// +------------------------+------------------+----------------------+
// | What runs it           | GTest directly   | CMake test runner    |
// | Output format          | GTest format     | CTest + GTest format |
// | Filter tests           | --gtest_filter=  | -R MyTest            |
// | Parallel tests         | NO               | YES (-j8)            |
// | CI/CD integration      | manual           | standard             |
// | XML reports            | --gtest_output=  | built in             |
// | Timeout per test       | NO               | YES                  |
// | Multiple test targets  | NO (one binary)  | YES (all registered) |
// | Requires enable_testing| NO               | YES                  |
// +------------------------+------------------+----------------------+


// TEST(Test_suite_name, Individuel_test_name)
//		Test_suite_name = Class name + Test
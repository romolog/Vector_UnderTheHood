#include "MyVector.hpp"
#include <gtest/gtest.h>
#include <iostream>

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

class ThrowClass
{
	private:
		static int count_;
		char* ptr;

	public:
		ThrowClass()
		{
			++count_;
			// std::cout << "ThrowClass Default Ctor: " << count_ << "\n";
			if (count_ % 3)
				throw std::runtime_error("ThrowClass Default Ctor");
			ptr = new char[42];
		};


		~ThrowClass(){ delete[] ptr;};

};

// template<class Vec>
// void TestClearVector(Vec& t)
// {
// 	t.clear();
// 	EXPECT_EQ(t.size(), 0);
// 	EXPECT_EQ(t.capacity(), 0);
// 	EXPECT_TRUE(t.empty());
// };


template<class Vec>
void TestEmptyVector(const Vec& t)
{
	// EXPECT_NE(t.data(), nullptr); // ASSERT_NE(ptr, nullptr) = stop if fail

	EXPECT_EQ(t.size(), 0);
	EXPECT_EQ(t.capacity(), 0);
	EXPECT_TRUE(t.empty());
	// EXPECT_EQ(t.begin(), t.end());
	// EXPECT_EQ(t.cbegin(), t.cend());
	// EXPECT_EQ(t.rbegin(), t.rend());
	// EXPECT_EQ(t.crbegin(), t.crend());

	// int count = 0;
    // for (int x : t)
    //     ++count;
    // EXPECT_EQ(count, 0);

};

// Empty Vector //-----------------------------------------------------------------

TEST(EmptyVector, DefaultCtor_Int) 
{	
    
	myvec::MyVector<int> t;
	TestEmptyVector(t);
	t.clear();
	TestEmptyVector(t);

	const myvec::MyVector<int> ct;
	TestEmptyVector(ct);
	// ct.clear(); // OK : compile error

	myvec::MyVector<int> copy(t);
	TestEmptyVector(copy);

	myvec::MyVector<int> src_move;
	myvec::MyVector<int> move(std::move(src_move));
	TestEmptyVector(move);
}


TEST(EmptyVector, DefaultCtor_ThrowClass) 
{	
    
	myvec::MyVector<ThrowClass> t;
	TestEmptyVector(t);
	t.clear();
	TestEmptyVector(t);

	const myvec::MyVector<ThrowClass> ct;
	TestEmptyVector(ct);
	// ct.clear(); // OK : compile error

	myvec::MyVector<ThrowClass> copy(t);
	TestEmptyVector(copy);

	myvec::MyVector<ThrowClass> src_move;
	myvec::MyVector<ThrowClass> move(std::move(src_move));
	TestEmptyVector(move);
}


TEST(EmptyVector, DefaultCtor_Bool) 
{	
    
	myvec::MyVector<bool> t;
	TestEmptyVector(t);
	t.clear();
	TestEmptyVector(t);

	const myvec::MyVector<bool> ct;
	TestEmptyVector(ct);
	// ct.clear(); // OK : compile error

	myvec::MyVector<bool> copy(t);
	TestEmptyVector(copy);

	myvec::MyVector<bool> src_move;
	myvec::MyVector<bool> move(std::move(src_move));
	TestEmptyVector(move);
}

// Size //-----------------------------------------------------------------

TEST(Vector42, SizeCtor_Int) 
{
	myvec::MyVector<int> t(42);
	EXPECT_EQ(t.size(), 42);
	EXPECT_GE(t.capacity(), 42); 
	EXPECT_FALSE(t.empty())	;
	t.clear();
	TestEmptyVector(t);
}

// Iter //-----------------------------------------------------------------

TEST(Iter, Int) {
	std::vector<int> stdv {24, 42, 43, 4224, 73};
	myvec::MyVector<int> v(stdv.begin(), stdv.end());
	EXPECT_EQ(v.size(), stdv.size());
	EXPECT_GE(v.capacity(), stdv.size()); 
	EXPECT_FALSE(v.empty())	;

	auto it = v.begin();
	auto end = v.end();
	EXPECT_NE(it, end);
	EXPECT_EQ(end - it, stdv.size());
	EXPECT_EQ(it - it, 0);
	EXPECT_EQ(end - end, 0);

	EXPECT_EQ(*it, stdv[0]);
	EXPECT_EQ(*(++it), stdv[1]);
	EXPECT_EQ(*(it++), stdv[1]);
	EXPECT_EQ(*it, stdv[2]);
	EXPECT_EQ(*(it + 2), stdv[4]);
	EXPECT_EQ(*(2 + it), stdv[4]);
	it += 2;
	EXPECT_EQ(*it, stdv[4]);
	++it;
	EXPECT_EQ(it, end);

	EXPECT_EQ(*(--it), stdv[4]);
	EXPECT_EQ(*(it--), stdv[4]);
	EXPECT_EQ(*it, stdv[3]);
	EXPECT_EQ(*(it - 3), stdv[0]);
	it -= 3;
	EXPECT_EQ(*it, stdv[0]);

	EXPECT_EQ(it[0], stdv[0]);
	EXPECT_EQ(it[3], stdv[3]);	
	EXPECT_EQ(it[4], stdv[4]);

}

// Init List //-----------------------------------------------------------------

TEST(InitList, Int) {
	myvec::MyVector<int> v{24, 42, 4224};
	EXPECT_EQ(v.size(), 3);
	EXPECT_GE(v.capacity(), 3); 
	EXPECT_FALSE(v.empty())	;
}


// TEST(MyVecTest, DefaultCtorCreatesEmptyVectorBool) {
// 	myvec::MyVector<bool> v;
// 	EXPECT_EQ(v.size(), 0);
// 	EXPECT_EQ(v.capacity(), 0);
// 	EXPECT_NE(v.data(), nullptr); // ASSERT_NE(ptr, nullptr) = stop if fail
// 	EXPECT_TRUE(v.empty());
// }



// TEST(MyVecTest, PushBack) {
//     myvec::MyVector<int> v;
//     v.push_back(1);
//     EXPECT_EQ(v.size(), 1);
// }


// int main(void)
// {
// 	myvec::MyVector<int> a;

// 	return 0;
// }
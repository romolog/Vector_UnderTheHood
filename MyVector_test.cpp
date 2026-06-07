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

// GTEST_LOG_(INFO) << "\n\tCall default ctor, check if size = 0, empty = true\n";
// EXPECT_NE(t.data(), nullptr); // ASSERT_NE(ptr, nullptr) = stop if fail
template<class Vec>
void TestEmptyVector(Vec& t)
{
	EXPECT_EQ(t.size(), 0);
	EXPECT_EQ(t.capacity(), 0);
	EXPECT_TRUE(t.empty());
	EXPECT_EQ(t.begin(), t.end());
	EXPECT_EQ(t.cbegin(), t.cend());
	EXPECT_EQ(t.rbegin(), t.rend());
	EXPECT_EQ(t.crbegin(), t.crend());

	int count = 0;
	for (auto&& _ : t)
	{
		(void)_;
		++count;
	}
	EXPECT_EQ(count, 0);
};

template <class T>
void RunEmptyVectorTests (void) 
{	
	// VALIDATE TEST ON STD LIB
	std::vector<T> st;
	TestEmptyVector(st);

	// EMPTY VECTOR
	myvec::MyVector<T> t;
	TestEmptyVector(t);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T> ct;
	TestEmptyVector(ct);
	// ct.clear(); // OK : compile error

	// COPY
	myvec::MyVector<T> src_mutable_copy;
	TestEmptyVector(src_mutable_copy);
	const myvec::MyVector<T> const_copy{src_mutable_copy};
	TestEmptyVector(const_copy);

	// COPY FROM CONST
	const myvec::MyVector<T> src_const_copy;
	TestEmptyVector(src_const_copy);
	myvec::MyVector<T> mutable_copy{src_const_copy};
	TestEmptyVector(mutable_copy);

	// MOVE
	myvec::MyVector<T> src_move_mutable;
	TestEmptyVector(src_move_mutable);
	myvec::MyVector<T> move{std::move(src_move_mutable)};
	TestEmptyVector(move);

	// MOVE(->COPY) FROM CONST
	const myvec::MyVector<T> src_const_refref;
	TestEmptyVector(src_const_refref);
	myvec::MyVector<T> copy_from_const_refref{std::move(src_const_refref)};
	TestEmptyVector(copy_from_const_refref);

	// COPY ASSIGN
	myvec::MyVector<T> src_copy_assign;
	TestEmptyVector(src_copy_assign);	
	myvec::MyVector<T> copy_assign;
	TestEmptyVector(copy_assign);
	copy_assign = src_copy_assign;
	TestEmptyVector(copy_assign);

	// COPY ASSIGN FROM CONST
	const myvec::MyVector<T> src_const_copy_assign;
	TestEmptyVector(src_const_copy_assign);	
	myvec::MyVector<T> assign_copy_from_const;
	TestEmptyVector(assign_copy_from_const);
	assign_copy_from_const = src_const_copy_assign;
	TestEmptyVector(assign_copy_from_const);

	// COPY ASSIGN SELF
	myvec::MyVector<T> copy_self;
	TestEmptyVector(copy_self);
	copy_self = copy_self;
	TestEmptyVector(copy_self);

	// MOVE ASSIGN
	myvec::MyVector<T> src_move_assign;
	TestEmptyVector(src_move_assign);	
	myvec::MyVector<T> move_assign;
	TestEmptyVector(move_assign);
	move_assign = std::move(src_move_assign);
	TestEmptyVector(move_assign);

	// MOVE ASSIGN SELF
	myvec::MyVector<T> move_self;
	TestEmptyVector(move_self);
	move_self = std::move(move_self);
	TestEmptyVector(move_self);
}

template<class Vec>
void TestVector(Vec& t, size_t size, size_t capacity)
{
	EXPECT_EQ(t.size(), size);
	EXPECT_GE(t.capacity(), capacity);
	EXPECT_FALSE(t.empty());
	EXPECT_NE(t.begin(), t.end());
	EXPECT_NE(t.cbegin(), t.cend());
	EXPECT_NE(t.rbegin(), t.rend());
	EXPECT_NE(t.crbegin(), t.crend());

	EXPECT_EQ(t.end() - t.begin(), size);
	EXPECT_EQ(t.cend() - t.cbegin(), size);
	EXPECT_EQ(t.rend() - t.rbegin(), size);
	EXPECT_EQ(t.crend() - t.crbegin(), size);

	int count = 0;
	for (auto&& _ : t)
	{
		(void)_;
		++count;
	}
	EXPECT_EQ(count, size);
};


// Empty Vector //-----------------------------------------------------------------

TEST(EmptyVector, Default_Int) 
{ RunEmptyVectorTests<int>(); }


TEST(EmptyVector, Default_StdVecOfString) 
{ RunEmptyVectorTests<std::vector<std::string> >(); }


// Empty Vector //-----------------------------------------------------------------


// Clear //-----------------------------------------------------------------

TEST(Clear, Int) 
{	
	using T = int;

	myvec::MyVector<T> t;
	TestEmptyVector(t);
	t.clear();
	TestEmptyVector(t);

	const myvec::MyVector<T> ct;
	TestEmptyVector(ct);
	// ct.clear(); // OK : compile error
}

// TEST(EmptyVector, DefaultCtor_ThrowClass) 
// {	
    
// 	myvec::MyVector<ThrowClass> t;
// 	TestEmptyVector(t);
// 	t.clear();
// 	TestEmptyVector(t);

// 	const myvec::MyVector<ThrowClass> ct;
// 	TestEmptyVector(ct);
// 	// ct.clear(); // OK : compile error

// 	myvec::MyVector<ThrowClass> copy(t);
// 	TestEmptyVector(copy);

// 	myvec::MyVector<ThrowClass> src_move;
// 	myvec::MyVector<ThrowClass> move(std::move(src_move));
// 	TestEmptyVector(move);
// }


// TEST(EmptyVector, DefaultCtor_Bool) 
// {	
    
// 	myvec::MyVector<bool> t;
// 	TestEmptyVector(t);
// 	t.clear();
// 	TestEmptyVector(t);

// 	const myvec::MyVector<bool> ct;
// 	TestEmptyVector(ct);
// 	// ct.clear(); // OK : compile error

// 	myvec::MyVector<bool> copy(t);
// 	TestEmptyVector(copy);

// 	myvec::MyVector<bool> src_move;
// 	myvec::MyVector<bool> move(std::move(src_move));
// 	TestEmptyVector(move);
// }

// Size //-----------------------------------------------------------------

TEST(Vector42, SizeCtor_Int) 
{
	myvec::MyVector<int> t(42);
	EXPECT_EQ(t.size(), 42);
	EXPECT_EQ(t.capacity(), 42); 
	EXPECT_FALSE(t.empty())	;
	t.clear();
	TestEmptyVector(t);
}

// Reserve //-----------------------------------------------------------------

TEST(EmptyVector_Int, Reserve) 
{
	myvec::MyVector<int> t;
	EXPECT_EQ(t.capacity(), 0);
	size_t n = 42;
	t.reserve(n);
	EXPECT_GE(t.capacity(), n); 
}

// Push back //-----------------------------------------------------------------

TEST(EmptyVector_Int, PushBack) 
{
	myvec::MyVector<int> t;
	EXPECT_EQ(t.capacity(), 0);
	int val = 42;
	t.push_back(val);
	EXPECT_EQ(t.capacity(), 1);
	EXPECT_EQ(t[0], val); 
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

// Const Iter //-----------------------------------------------------------------

TEST(ConstIter, Int) {
	myvec::MyVector<int> v{24, 42, 43, 4224, 73};
	auto cit = v.cbegin();
	EXPECT_EQ(*cit, v[0]);

}


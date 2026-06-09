#include "MyVector.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <gmock/gmock.h> // EXPECT_THAT
#include <iostream>
#include <random>
#include <string>
#include <vector>


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


std::initializer_list<int> il_3int = {37, 42, 73};
std::initializer_list<int> il_7int = {1, 2, 3, 4, 5, 6, 7};

std::initializer_list<std::string> il_3str = {"a", "ab", "abc"};
std::initializer_list<std::string> il_7str = {"Hello", "World", "!", "hello_world", "Hello World !", "Hi", "Good day"};


// this to check number of copies of elements
struct Counter
{
	//	inline allows to define in header
	//	I still can change variable
    static inline int copy_ctor = 0;

    Counter() = default;

    Counter(const Counter&) { ++copy_ctor; }
};


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

template<class Vec>
void TestSizeVector(Vec& t, size_t size)
{

	EXPECT_EQ(t.size(), size);
	EXPECT_GE(t.capacity(), size);
	
	if (size)
	{
		EXPECT_FALSE(t.empty());
		EXPECT_NE(t.begin(), t.end());
		EXPECT_NE(t.cbegin(), t.cend());
		EXPECT_NE(t.rbegin(), t.rend());
		EXPECT_NE(t.crbegin(), t.crend());
	}
	else
		EXPECT_TRUE(t.empty());

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

template<class Vec>
void TestSizeValVector(Vec& t, size_t size, const typename Vec::value_type& val)
{

	TestSizeVector(t, size);

	std::for_each(t.begin(), t.end(), [val](auto&& el){EXPECT_EQ(el, val);});
	// EXPECT_EQ(t[0], val);

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

template <class T>
void RunZeroVectorTests (void) 
{	
	// VALIDATE TEST ON STD LIB
	std::vector<T> st(0);
	TestEmptyVector(st);

	// EMPTY VECTOR
	myvec::MyVector<T> t(0);
	TestEmptyVector(t);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T> ct(0);
	TestEmptyVector(ct);
	// ct.clear(); // OK : compile error

	// COPY
	myvec::MyVector<T> src_mutable_copy(0);
	TestEmptyVector(src_mutable_copy);
	const myvec::MyVector<T> const_copy{src_mutable_copy};
	TestEmptyVector(const_copy);

	// COPY FROM CONST
	const myvec::MyVector<T> src_const_copy(0);
	TestEmptyVector(src_const_copy);
	myvec::MyVector<T> mutable_copy{src_const_copy};
	TestEmptyVector(mutable_copy);

	// MOVE
	myvec::MyVector<T> src_move_mutable(0);
	TestEmptyVector(src_move_mutable);
	myvec::MyVector<T> move{std::move(src_move_mutable)};
	TestEmptyVector(move);

	// MOVE(->COPY) FROM CONST
	const myvec::MyVector<T> src_const_refref(0);
	TestEmptyVector(src_const_refref);
	myvec::MyVector<T> copy_from_const_refref{std::move(src_const_refref)};
	TestEmptyVector(copy_from_const_refref);

	// COPY ASSIGN
	myvec::MyVector<T> src_copy_assign(0);
	TestEmptyVector(src_copy_assign);	
	myvec::MyVector<T> copy_assign(0);
	TestEmptyVector(copy_assign);
	copy_assign = src_copy_assign;
	TestEmptyVector(copy_assign);

	// COPY ASSIGN FROM CONST
	const myvec::MyVector<T> src_const_copy_assign(0);
	TestEmptyVector(src_const_copy_assign);	
	myvec::MyVector<T> assign_copy_from_const(0);
	TestEmptyVector(assign_copy_from_const);
	assign_copy_from_const = src_const_copy_assign;
	TestEmptyVector(assign_copy_from_const);

	// COPY ASSIGN SELF
	myvec::MyVector<T> copy_self(0);
	TestEmptyVector(copy_self);
	copy_self = copy_self;
	TestEmptyVector(copy_self);

	// MOVE ASSIGN
	myvec::MyVector<T> src_move_assign(0);
	TestEmptyVector(src_move_assign);	
	myvec::MyVector<T> move_assign(0);
	TestEmptyVector(move_assign);
	move_assign = std::move(src_move_assign);
	TestEmptyVector(move_assign);

	// MOVE ASSIGN SELF
	myvec::MyVector<T> move_self(0);
	TestEmptyVector(move_self);
	move_self = std::move(move_self);
	TestEmptyVector(move_self);
}

template <class T>
void RunSizeVectorTests (size_t size) 
{	
	ASSERT_GT(size, 0);

	// VALIDATE TEST ON STD LIB
	std::vector<T> st(size);
	TestSizeVector(st, size);

	// EMPTY VECTOR
	myvec::MyVector<T> t(size);
	TestSizeVector(t, size);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T> ct(size);
	TestSizeVector(ct, size);
	// ct.clear(); // OK : compile error

	// COPY
	myvec::MyVector<T> src_mutable_copy(size);
	TestSizeVector(src_mutable_copy, size);
	const myvec::MyVector<T> const_copy{src_mutable_copy};
	TestSizeVector(const_copy, size);

	// COPY FROM CONST
	const myvec::MyVector<T> src_const_copy(size);
	TestSizeVector(src_const_copy, size);
	myvec::MyVector<T> mutable_copy{src_const_copy};
	TestSizeVector(mutable_copy, size);

	// MOVE
	myvec::MyVector<T> src_move_mutable(size);
	TestSizeVector(src_move_mutable, size);
	myvec::MyVector<T> move{std::move(src_move_mutable)};
	TestSizeVector(move, size);

	// MOVE(->COPY) FROM CONST
	const myvec::MyVector<T> src_const_refref(size);
	TestSizeVector(src_const_refref, size);
	myvec::MyVector<T> copy_from_const_refref{std::move(src_const_refref)};
	TestSizeVector(copy_from_const_refref, size);

	// COPY ASSIGN
	myvec::MyVector<T> src_copy_assign(size);
	TestSizeVector(src_copy_assign, size);	
	myvec::MyVector<T> copy_assign(size);
	TestSizeVector(copy_assign, size);
	copy_assign = src_copy_assign;
	TestSizeVector(copy_assign, size);

	// COPY ASSIGN FROM CONST
	const myvec::MyVector<T> src_const_copy_assign(size);
	TestSizeVector(src_const_copy_assign, size);	
	myvec::MyVector<T> assign_copy_from_const(size);
	TestSizeVector(assign_copy_from_const, size);
	assign_copy_from_const = src_const_copy_assign;
	TestSizeVector(assign_copy_from_const, size);

	// COPY ASSIGN SELF
	myvec::MyVector<T> copy_self(size);
	TestSizeVector(copy_self, size);
	copy_self = copy_self;
	TestSizeVector(copy_self, size);

	// MOVE ASSIGN
	myvec::MyVector<T> src_move_assign(size);
	TestSizeVector(src_move_assign, size);	
	myvec::MyVector<T> move_assign(size);
	TestSizeVector(move_assign, size);
	move_assign = std::move(src_move_assign);
	TestSizeVector(move_assign, size);

	// MOVE ASSIGN SELF
	myvec::MyVector<T> move_self(size);
	TestSizeVector(move_self, size);
	move_self = std::move(move_self);
	TestSizeVector(move_self, size);
}

template <class T>
void RunSizeValVectorTests (size_t size, T value) 
{	
	// VALIDATE TEST ON STD LIB
	std::vector<T> st(size, value);
	TestSizeValVector(st, size, value);

	// EMPTY VECTOR
	myvec::MyVector<T> t(size, value);
	TestSizeValVector(t, size, value);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T> ct(size, value);
	TestSizeValVector(ct, size, value);
	// ct.clear(); // OK : compile error

	// COPY
	myvec::MyVector<T> src_mutable_copy(size, value);
	TestSizeValVector(src_mutable_copy, size, value);
	const myvec::MyVector<T> const_copy{src_mutable_copy};
	TestSizeValVector(const_copy, size, value);

	// COPY FROM CONST
	const myvec::MyVector<T> src_const_copy(size, value);
	TestSizeValVector(src_const_copy, size, value);
	myvec::MyVector<T> mutable_copy{src_const_copy};
	TestSizeValVector(mutable_copy, size, value);

	// MOVE
	myvec::MyVector<T> src_move_mutable(size, value);
	TestSizeValVector(src_move_mutable, size, value);
	myvec::MyVector<T> move{std::move(src_move_mutable)};
	TestSizeValVector(move, size, value);

	// MOVE(->COPY) FROM CONST
	const myvec::MyVector<T> src_const_refref(size, value);
	TestSizeValVector(src_const_refref, size, value);
	myvec::MyVector<T> copy_from_const_refref{std::move(src_const_refref)};
	TestSizeValVector(copy_from_const_refref, size, value);

	// COPY ASSIGN
	myvec::MyVector<T> src_copy_assign(size, value);
	TestSizeValVector(src_copy_assign, size, value);	
	myvec::MyVector<T> copy_assign(size, value);
	TestSizeValVector(copy_assign, size, value);
	copy_assign = src_copy_assign;
	TestSizeValVector(copy_assign, size, value);

	// COPY ASSIGN FROM CONST
	const myvec::MyVector<T> src_const_copy_assign(size, value);
	TestSizeValVector(src_const_copy_assign, size, value);	
	myvec::MyVector<T> assign_copy_from_const(size, value);
	TestSizeValVector(assign_copy_from_const, size, value);
	assign_copy_from_const = src_const_copy_assign;
	TestSizeValVector(assign_copy_from_const, size, value);

	// COPY ASSIGN SELF
	myvec::MyVector<T> copy_self(size, value);
	TestSizeValVector(copy_self, size, value);
	copy_self = copy_self;
	TestSizeValVector(copy_self, size, value);

	// MOVE ASSIGN
	myvec::MyVector<T> src_move_assign(size, value);
	TestSizeValVector(src_move_assign, size, value);	
	myvec::MyVector<T> move_assign(size, value);
	TestSizeValVector(move_assign, size, value);
	move_assign = std::move(src_move_assign);
	TestSizeValVector(move_assign, size, value);

	// MOVE ASSIGN SELF
	myvec::MyVector<T> move_self(size, value);
	TestSizeValVector(move_self, size, value);
	move_self = std::move(move_self);
	TestSizeValVector(move_self, size, value);
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


// Empty Vector Allocator //-----------------------------------------------------------------

TEST(EmptyVector, Allocator_Int) 
{
	using T = int;

	// VALIDATE TEST ON STD LIB
	std::vector<T, std::allocator<T> > st;
	TestEmptyVector(st);

	// EMPTY VECTOR
	myvec::MyVector<T, std::allocator<T>> t;
	TestEmptyVector(t);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T, std::allocator<T>> ct;
	TestEmptyVector(ct);
}

// SizeZero Vector //-----------------------------------------------------------------

TEST(EmptyVector, SizeZero_Int) 
{ RunZeroVectorTests<int>(); }


TEST(EmptyVector, SizeZero_StdVecOfString) 
{ RunZeroVectorTests<std::vector<std::string> >(); }


// ZeroSize Value Vector //-----------------------------------------------------------------

TEST(EmptyVector, SizeZero_SomeVal_Int) 
{ RunSizeValVectorTests<int>(0, 42); }


TEST(EmptyVector,  SizeZero_SomeVal_StdVecOfString) 
{ RunSizeValVectorTests<std::vector<int> >(0, std::vector<int>{42, 73}); }

// Size42 Vector //-----------------------------------------------------------------

TEST(Ctor_Size42___, Default_Int) 
{ RunSizeVectorTests<int>(42); }

TEST(Ctor_Size42___, Size42_VectorInt) 
{ RunSizeVectorTests< std::vector<int> >(42); }


// Size42 Value Vector //-----------------------------------------------------------------

TEST(Ctor_Size42___, Size42_SomeVal_Int) 
{ RunSizeValVectorTests<int>(42, 73); }


TEST(Ctor_Size42___,  Size42_SomeVal_StdVecOfString) 
{ RunSizeValVectorTests<std::vector<int> >(42, std::vector<int>{42, 73}); }

// InputIt Vector //-----------------------------------------------------------------

TEST(Ctor_InputIt___, Vector_Int) 
{ 
	using T = int;

	std::vector<T> v(42);
	std::iota(v.begin(), v.end(), 37);
	std::random_device rd;
	std::mt19937 gen(rd()); // gen(73) if deterministic needed
	std::shuffle(v.begin(), v.end(), gen);

	myvec::MyVector<T> mv(v.begin(), v.end());
	
	EXPECT_EQ(v.size(), mv.size());
	EXPECT_EQ(std::distance(v.begin(), v.end()), std::distance(mv.begin(), mv.end()));
	// EXPECT_TRUE(std::equal(v.begin(), v.end(), mv.begin()));
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

	myvec::MyVector<T> mv_copy(mv.begin(), mv.end());
	EXPECT_EQ(v.size(), mv_copy.size());
	EXPECT_TRUE(std::equal(v.begin(), v.end(), mv_copy.begin()));
}


TEST(Ctor_InputIt___, Vector_String) 
{ 
	using T = std::string;

	std::vector<T> v {il_7str};

	myvec::MyVector<T> mv(v.begin(), v.end()); 
	
	EXPECT_EQ(v.size(), mv.size());
	EXPECT_EQ(std::distance(v.begin(), v.end()), std::distance(mv.begin(), mv.end()));
	EXPECT_TRUE(std::equal(v.begin(), v.end(), mv.begin()));
	EXPECT_STREQ(v[0].c_str(), mv[0].c_str()); // just to show, that const char* comparison is different in GTest
	
	myvec::MyVector<T> mv_copy(mv.begin(), mv.end());
	EXPECT_EQ(v.size(), mv_copy.size());
	EXPECT_TRUE(std::equal(v.begin(), v.end(), mv_copy.begin()));
}

// Initializer List //-----------------------------------------------------------------


TEST(Ctor_InitializerList___, Int_Zero) 
{ 
	using T = int;
	std::initializer_list<T> inil = {};

	myvec::MyVector<T> mv {inil};
	EXPECT_EQ(mv.size(), inil.size());
	EXPECT_GE(mv.capacity(), inil.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
	std::vector<T> v {inil};
	EXPECT_EQ(mv.size(), v.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

}

TEST(Ctor_InitializerList___, Int_Single) 
{ 
	using T = int;
	std::initializer_list<T> inil = {42};

	myvec::MyVector<T> mv {inil};
	EXPECT_EQ(mv.size(), inil.size());
	EXPECT_GE(mv.capacity(), inil.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
	std::vector<T> v {inil};
	EXPECT_EQ(mv.size(), v.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

}


TEST(Ctor_InitializerList___, Int_Multiple) 
{ 
	using T = int;
	std::initializer_list<T> inil = il_7int;

	myvec::MyVector<T> mv {inil};
	EXPECT_EQ(mv.size(), inil.size());
	EXPECT_GE(mv.capacity(), inil.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
	std::vector<T> v {inil};
	EXPECT_EQ(mv.size(), v.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

}

TEST(Ctor_InitializerList___, String_Zero) 
{ 
	using T = std::string;
	std::initializer_list<T> inil = {};

	myvec::MyVector<T> mv {inil};
	EXPECT_EQ(mv.size(), inil.size());
	EXPECT_GE(mv.capacity(), inil.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
	std::vector<T> v {inil};
	EXPECT_EQ(mv.size(), v.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

}

TEST(Ctor_InitializerList___, String_Single) 
{ 
	using T = std::string;
	std::initializer_list<T> inil = {"Hello"};

	myvec::MyVector<T> mv {inil};
	EXPECT_EQ(mv.size(), inil.size());
	EXPECT_GE(mv.capacity(), inil.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
	std::vector<T> v {inil};
	EXPECT_EQ(mv.size(), v.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

}

TEST(Ctor_InitializerList___, String_Multiple) 
{ 
	using T = std::string;
	std::initializer_list<T> inil = il_7str;

	myvec::MyVector<T> mv {inil};
	EXPECT_EQ(mv.size(), inil.size());
	EXPECT_GE(mv.capacity(), inil.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
	std::vector<T> v {inil};
	EXPECT_EQ(mv.size(), v.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

}

TEST(Ctor_InitializerList___, Const_Int) 
{ 
	using T = int;
	const std::initializer_list<T> inil = il_7int;

	const myvec::MyVector<T> mv {inil};
	EXPECT_EQ(mv.size(), inil.size());
	EXPECT_GE(mv.capacity(), inil.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
	std::vector<T> v {inil};
	EXPECT_EQ(mv.size(), v.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

}

TEST(Ctor_InitializerList___, Const_String) 
{ 
	using T = std::string;
	const std::initializer_list<T> inil = il_7str;

	const myvec::MyVector<T> mv {inil};
	EXPECT_EQ(mv.size(), inil.size());
	EXPECT_GE(mv.capacity(), inil.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
	std::vector<T> v {inil};
	EXPECT_EQ(mv.size(), v.size());
	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

}

// Move //-----------------------------------------------------------------

TEST(Ctor_Move___, Int) 
{ 
	using T = int;
	std::initializer_list<T> inil = il_7int;

	myvec::MyVector<T> src_move {inil};
	EXPECT_EQ(src_move.size(), inil.size());
	EXPECT_GE(src_move.capacity(), inil.size());
	EXPECT_THAT( src_move, ::testing::ElementsAreArray(inil));
	// EXPECT_THAT( src_move, ::testing::ElementsAre(37, 43, 73)); // much worse, needs manual input
	// EXPECT_TRUE(std::equal(src_move.begin(), src_move.end(), inil.begin())); // I can do it
	// RAW LOOP: I can do it
	// auto comparator = 	[&inil](auto&& el)
	// 					{ 
	// 						static auto it = inil.begin(); 
	// 						EXPECT_EQ(el, *it);
	// 						++it;
	// 					};
	// std::for_each(src_move.begin(), src_move.end(), comparator);

	myvec::MyVector<T> move (std::move(src_move));
	EXPECT_EQ(move.size(), inil.size());
	EXPECT_GE(move.capacity(), inil.size());
	EXPECT_TRUE(std::equal(move.begin(), move.end(), inil.begin()));
	
	EXPECT_TRUE(src_move.empty());
	EXPECT_EQ(src_move.size(), 0);
	EXPECT_GE(src_move.capacity(), 0);
	EXPECT_EQ(src_move.data(), nullptr);

}

TEST(Ctor_Copy___, Move_Const_Int) 
{ 
	using T = int;
	const std::initializer_list<T> inil = il_7int;

	const myvec::MyVector<T> src_move {inil};
	EXPECT_EQ(src_move.size(), inil.size());
	EXPECT_GE(src_move.capacity(), inil.size());
	EXPECT_THAT( src_move, ::testing::ElementsAreArray(inil));
	
	myvec::MyVector<T> move (std::move(src_move));
	EXPECT_EQ(move.size(), inil.size());
	EXPECT_GE(move.capacity(), inil.size());
	EXPECT_TRUE(std::equal(move.begin(), move.end(), inil.begin()));
}


TEST(Ctor_Move___, _String) 
{ 
	using T = std::string;
	std::initializer_list<T> inil = il_7str;

	myvec::MyVector<T> src_move {inil};
	EXPECT_EQ(src_move.size(), inil.size());
	EXPECT_GE(src_move.capacity(), inil.size());
	EXPECT_THAT( src_move, ::testing::ElementsAreArray(inil));

	
	myvec::MyVector<T> move (std::move(src_move));
	EXPECT_EQ(move.size(), inil.size());
	EXPECT_GE(move.capacity(), inil.size());
	EXPECT_TRUE(std::equal(move.begin(), move.end(), inil.begin()));
	
	EXPECT_TRUE(src_move.empty());
	EXPECT_EQ(src_move.size(), 0);
	EXPECT_GE(src_move.capacity(), 0);
	EXPECT_EQ(src_move.data(), nullptr);
}

// Move Allocator //-----------------------------------------------------------------

		// TODO add with custom allocators


// Move Assignment //-----------------------------------------------------------------

TEST(Assignment_Move___, Int) 
{ 
	using T = int;
	std::initializer_list<T> inil_1 = il_3int;
	std::initializer_list<T> inil_2 = il_7int;

	myvec::MyVector<T> move_1 {inil_1};
	EXPECT_EQ(move_1.size(), inil_1.size());
	EXPECT_GE(move_1.capacity(), inil_1.size());
	EXPECT_THAT(move_1, ::testing::ElementsAreArray(inil_1));
	
	myvec::MyVector<T> move_2 {inil_2};
	move_2 = (std::move(move_1));

	EXPECT_FALSE(move_1.empty());
	EXPECT_EQ(move_1.size(), inil_2.size());
	EXPECT_GE(move_1.capacity(), inil_2.size());
	EXPECT_TRUE(std::equal(move_1.begin(), move_1.end(), inil_2.begin()));
	
	EXPECT_FALSE(move_2.empty());
	EXPECT_EQ(move_2.size(), inil_1.size());
	EXPECT_GE(move_2.capacity(), inil_1.size());
	EXPECT_TRUE(std::equal(move_2.begin(), move_2.end(), inil_1.begin()));

}


TEST(Assignment_Move___, String) 
{ 
	using T = std::string;
	std::initializer_list<T> inil_1 = il_3str;
	std::initializer_list<T> inil_2 = il_7str;

	myvec::MyVector<T> move_1 {inil_1};
	EXPECT_EQ(move_1.size(), inil_1.size());
	EXPECT_GE(move_1.capacity(), inil_1.size());
	EXPECT_THAT(move_1, ::testing::ElementsAreArray(inil_1));
	
	myvec::MyVector<T> move_2 {inil_2};
	move_2 = (std::move(move_1));

	EXPECT_FALSE(move_1.empty());
	EXPECT_EQ(move_1.size(), inil_2.size());
	EXPECT_GE(move_1.capacity(), inil_2.size());
	EXPECT_TRUE(std::equal(move_1.begin(), move_1.end(), inil_2.begin()));
	
	EXPECT_FALSE(move_2.empty());
	EXPECT_EQ(move_2.size(), inil_1.size());
	EXPECT_GE(move_2.capacity(), inil_1.size());
	EXPECT_TRUE(std::equal(move_2.begin(), move_2.end(), inil_1.begin()));

}

TEST(Assignment_Move___, Self_Int) 
{ 
	using T = int;
	std::initializer_list<T> inil_1 = il_3int;

	myvec::MyVector<T> move_1 {inil_1};
	EXPECT_EQ(move_1.size(), inil_1.size());
	EXPECT_GE(move_1.capacity(), inil_1.size());
	EXPECT_THAT(move_1, ::testing::ElementsAreArray(inil_1));
	
	move_1 = (std::move(move_1));

	EXPECT_FALSE(move_1.empty());
	EXPECT_EQ(move_1.size(), inil_1.size());
	EXPECT_GE(move_1.capacity(), inil_1.size());
	EXPECT_TRUE(std::equal(move_1.begin(), move_1.end(), inil_1.begin()));

}

TEST(Assignment_Move___, Self_String) 
{ 
	using T = std::string;
	std::initializer_list<T> inil_1 = il_3str;

	myvec::MyVector<T> move_1 {inil_1};
	EXPECT_EQ(move_1.size(), inil_1.size());
	EXPECT_GE(move_1.capacity(), inil_1.size());
	EXPECT_THAT(move_1, ::testing::ElementsAreArray(inil_1));
	
	move_1 = (std::move(move_1));

	EXPECT_FALSE(move_1.empty());
	EXPECT_EQ(move_1.size(), inil_1.size());
	EXPECT_GE(move_1.capacity(), inil_1.size());
	EXPECT_TRUE(std::equal(move_1.begin(), move_1.end(), inil_1.begin()));

}

// Ctor Copy //-----------------------------------------------------------------

TEST(Ctor_Copy___, Int) 
{ 
	using T = int;
	std::initializer_list<T> inil_1 = il_3int;

	myvec::MyVector<T> src_copy {inil_1};
	EXPECT_EQ(src_copy.size(), inil_1.size());
	EXPECT_GE(src_copy.capacity(), inil_1.size());
	EXPECT_THAT(src_copy, ::testing::ElementsAreArray(inil_1));
	
	myvec::MyVector<T> copy(src_copy);

	EXPECT_EQ(src_copy.size(), inil_1.size());
	EXPECT_GE(src_copy.capacity(), inil_1.size());
	EXPECT_THAT(src_copy, ::testing::ElementsAreArray(inil_1));

	EXPECT_EQ(copy.size(), inil_1.size());
	EXPECT_GE(copy.capacity(), inil_1.size());
	EXPECT_THAT(copy, ::testing::ElementsAreArray(inil_1));
}

TEST(Ctor_Copy___, String) 
{ 
	using T = std::string;
	std::initializer_list<T> inil_1 = il_3str;

	myvec::MyVector<T> src_copy {inil_1};
	EXPECT_EQ(src_copy.size(), inil_1.size());
	EXPECT_GE(src_copy.capacity(), inil_1.size());
	EXPECT_THAT(src_copy, ::testing::ElementsAreArray(inil_1));
	
	myvec::MyVector<T> copy(src_copy);

	EXPECT_EQ(src_copy.size(), inil_1.size());
	EXPECT_GE(src_copy.capacity(), inil_1.size());
	EXPECT_THAT(src_copy, ::testing::ElementsAreArray(inil_1));

	EXPECT_EQ(copy.size(), inil_1.size());
	EXPECT_GE(copy.capacity(), inil_1.size());
	EXPECT_THAT(copy, ::testing::ElementsAreArray(inil_1));
}

// Ctor Copy Allocator //-----------------------------------------------------------------

	// TODO: add tests

// Copy Assignment //-----------------------------------------------------------------

TEST(Assignment_Copy___, Int) 
{ 
	using T = int;
	std::initializer_list<T> inil_1 = il_3int;
	std::initializer_list<T> inil_2 = il_7int;

	myvec::MyVector<T> copy_1 {inil_1};
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
	myvec::MyVector<T> copy_2 {inil_2};
	EXPECT_EQ(copy_2.size(), inil_2.size());
	EXPECT_GE(copy_2.capacity(), inil_2.size());
	EXPECT_TRUE(std::equal(copy_2.begin(), copy_2.end(), inil_2.begin()));
		
	copy_2 = copy_1;
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	EXPECT_EQ(copy_2.size(), inil_1.size());
	EXPECT_GE(copy_2.capacity(), inil_1.size());
	EXPECT_THAT(copy_2, ::testing::ElementsAreArray(inil_1));
}

TEST(Assignment_Copy___, String) 
{ 
	using T = std::string;
	std::initializer_list<T> inil_1 = il_3str;
	std::initializer_list<T> inil_2 = il_7str;

	myvec::MyVector<T> copy_1 {inil_1};
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
	myvec::MyVector<T> copy_2 {inil_2};
	EXPECT_EQ(copy_2.size(), inil_2.size());
	EXPECT_GE(copy_2.capacity(), inil_2.size());
	EXPECT_TRUE(std::equal(copy_2.begin(), copy_2.end(), inil_2.begin()));
		
	copy_2 = copy_1;
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	EXPECT_EQ(copy_2.size(), inil_1.size());
	EXPECT_GE(copy_2.capacity(), inil_1.size());
	EXPECT_THAT(copy_2, ::testing::ElementsAreArray(inil_1));
}


TEST(Assignment_Copy___, Self_Int) 
{ 
	using T = int;
	std::initializer_list<T> inil_1 = il_3int;

	myvec::MyVector<T> copy_1 {inil_1};
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
	copy_1 = copy_1;
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));

}

TEST(Assignment_Copy___, Self_String) 
{ 
	using T = std::string;
	std::initializer_list<T> inil_1 = il_3str;

	myvec::MyVector<T> copy_1 {inil_1};
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
	copy_1 = copy_1;
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
}

TEST(Assignment_Copy___, Zero_Int) 
{ 
	using T = int;
	std::initializer_list<T> inil_1 = {};
	std::initializer_list<T> inil_2 = {};

	myvec::MyVector<T> copy_1 {inil_1};
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
	myvec::MyVector<T> copy_2 {inil_2};
	EXPECT_EQ(copy_2.size(), inil_2.size());
	EXPECT_GE(copy_2.capacity(), inil_2.size());
	EXPECT_TRUE(std::equal(copy_2.begin(), copy_2.end(), inil_2.begin()));
		
	copy_2 = copy_1;
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	EXPECT_EQ(copy_2.size(), inil_1.size());
	EXPECT_GE(copy_2.capacity(), inil_1.size());
	EXPECT_THAT(copy_2, ::testing::ElementsAreArray(inil_1));
}

TEST(Assignment_Copy___, Zero_String) 
{ 
	using T = std::string;
	std::initializer_list<T> inil_1 = {};
	std::initializer_list<T> inil_2 = {};

	myvec::MyVector<T> copy_1 {inil_1};
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
	myvec::MyVector<T> copy_2 {inil_2};
	EXPECT_EQ(copy_2.size(), inil_2.size());
	EXPECT_GE(copy_2.capacity(), inil_2.size());
	EXPECT_TRUE(std::equal(copy_2.begin(), copy_2.end(), inil_2.begin()));
		
	copy_2 = copy_1;
	EXPECT_EQ(copy_1.size(), inil_1.size());
	EXPECT_GE(copy_1.capacity(), inil_1.size());
	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	EXPECT_EQ(copy_2.size(), inil_1.size());
	EXPECT_GE(copy_2.capacity(), inil_1.size());
	EXPECT_THAT(copy_2, ::testing::ElementsAreArray(inil_1));
}

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


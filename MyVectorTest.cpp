#include "MyVector.hpp"

#include "MyVectorTestClasses.hpp"
#include "MyVectorTestFixtures.hpp"
#include "MyVectorTestFunctions.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <gmock/gmock.h> // EXPECT_THAT
// #include <iostream>
#include <random>
#include <string>
#include <type_traits>
#include <concepts>
#include <vector>


using TestTypes = ::testing::Types	<
									int,
									std::string
									>;


TYPED_TEST_SUITE(TT_1, TestTypes);

// Ctor Default = Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Default___) 
{
	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;

	TestEmptyVector(mv);
	TestEmptyVector(v);
	
	EXPECT_EQ(mv.data(), nullptr);
	EXPECT_EQ(v.data(), nullptr);

	const myvec::MyVector<TypeParam> cmv;
	const std::vector<TypeParam> cv;
	
	TestEmptyVector(cmv);
	TestEmptyVector(cv);

	EXPECT_EQ(cmv.data(), nullptr);
	EXPECT_EQ(cv.data(), nullptr);
}

// Ctor Size 0 = Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_SizeZero___) 
{
	size_t size = 0;

	myvec::MyVector<TypeParam> mv(size);
	std::vector<TypeParam> v(size);

	TestSizeVector(mv, 0);
	TestSizeVector(v, 0);
	
	EXPECT_EQ(mv.data(), nullptr);
	EXPECT_EQ(v.data(), nullptr);

	EXPECT_EQ(mv.capacity(), 0);
	EXPECT_EQ(v.capacity(), 0);

	const myvec::MyVector<TypeParam> cmv(size);
	const std::vector<TypeParam> cv(size);
	
	TestSizeVector(mv, 0);
	TestSizeVector(v, 0);

	EXPECT_EQ(cmv.data(), nullptr);
	EXPECT_EQ(cv.data(), nullptr);

	EXPECT_EQ(cmv.capacity(), 0);
	EXPECT_EQ(cv.capacity(), 0);
}

// Ctor Size 1 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_SizeSingle___) 
{
	size_t size = 1;

	myvec::MyVector<TypeParam> mv(size);
	std::vector<TypeParam> v(size);

	TestSizeVector(mv, size);
	TestSizeVector(v, size);
	
	const myvec::MyVector<TypeParam> cmv(size);
	const std::vector<TypeParam> cv(size);
	
	TestSizeVector(cmv, size);
	TestSizeVector(cv, size);
}

// Ctor Size 42 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Size42___) 
{
	size_t size = 42;

	myvec::MyVector<TypeParam> mv(size);
	std::vector<TypeParam> v(size);

	TestSizeVector(mv, size);
	TestSizeVector(v, size);
	
	const myvec::MyVector<TypeParam> cmv(size);
	const std::vector<TypeParam> cv(size);
	
	TestSizeVector(cmv, size);
	TestSizeVector(cv, size);
}

// Ctor Size 0 Value 42 = Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_SizeZero_Value42___) 
{
	size_t size = 0;
	TypeParam val = this->value(42);

	myvec::MyVector<TypeParam> mv(size, val);
	std::vector<TypeParam> v(size, val);

	TestSizeVector(mv, size);
	TestSizeVector(v, size);
	
	EXPECT_EQ(mv.data(), nullptr);
	EXPECT_EQ(v.data(), nullptr);

	EXPECT_EQ(mv.capacity(), 0);
	EXPECT_EQ(v.capacity(), 0);

	const myvec::MyVector<TypeParam> cmv(size, val);
	const std::vector<TypeParam> cv(size, val);
	
	TestSizeVector(mv, size);
	TestSizeVector(v, size);

	EXPECT_EQ(cmv.data(), nullptr);
	EXPECT_EQ(cv.data(), nullptr);

	EXPECT_EQ(cmv.capacity(), 0);
	EXPECT_EQ(cv.capacity(), 0);
}


// Ctor Size 1 Value 42 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_SizeSingle_Value42___) 
{
	size_t size = 1;
	TypeParam val = this->value(42);

	myvec::MyVector<TypeParam> mv(size, val);
	std::vector<TypeParam> v(size, val);

	TestSizeValVector(mv, size, val);
	TestSizeValVector(v, size, val);
	
	const myvec::MyVector<TypeParam> cmv(size, val);
	const std::vector<TypeParam> cv(size, val);
	
	TestSizeValVector(cmv, size, val);
	TestSizeValVector(cv, size, val);
}

// Ctor Size 37 Value 42 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Size37_Value42___) 
{
	size_t size = 37;
	TypeParam val = this->value(42);

	myvec::MyVector<TypeParam> mv(size, val);
	std::vector<TypeParam> v(size, val);

	TestSizeValVector(mv, size, val);
	TestSizeValVector(v, size, val);
	
	const myvec::MyVector<TypeParam> cmv(size, val);
	const std::vector<TypeParam> cv(size, val);
	
	TestSizeValVector(cmv, size, val);
	TestSizeValVector(cv, size, val);
}

// Ctor Size 37 Value 42 : Size alias Value //-----------------------------------------------------------------

TEST(Special, Ctor_Size37_Value42_Alias___) 
{
	using TypeParam = size_t;
	TypeParam size = 37;
	TypeParam& val = size;

	myvec::MyVector<TypeParam> mv(size, val);
	std::vector<TypeParam> v(size, val);

	TestSizeValVector(mv, size, size);
	TestSizeValVector(v, size, size);
	
	const myvec::MyVector<TypeParam> cmv(size, val);
	const std::vector<TypeParam> cv(size, val);
	
	TestSizeValVector(cmv, size, size);
	TestSizeValVector(cv, size, size);
}

// Ctor InputIter from Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InputIter_Empty___) 
{
	std::vector<TypeParam> src;

	myvec::MyVector<TypeParam> mv(src.begin(), src.end());
	std::vector<TypeParam> v(src.begin(), src.end());

	TestEmptyVector(mv);
	TestEmptyVector(v);
	
	EXPECT_EQ(mv.data(), nullptr);
	EXPECT_EQ(v.data(), nullptr);
	
	const myvec::MyVector<TypeParam> cmv(src.begin(), src.end());
	const std::vector<TypeParam> cv(src.begin(), src.end());
	
	TestEmptyVector(cmv);
	TestEmptyVector(cv);
	
	EXPECT_EQ(cmv.data(), nullptr);
	EXPECT_EQ(cv.data(), nullptr);
}

// Ctor InputIter from Size 1 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InputIter_SizeSingle___) 
{
	size_t size = 1;
	TypeParam val = this->value(42);
	std::vector<TypeParam> src(1, val);

	myvec::MyVector<TypeParam> mv(src.begin(), src.end());
	std::vector<TypeParam> v(src.begin(), src.end());

	TestSizeValVector(mv, size, val);
	TestSizeValVector(v, size, val);
	
	const myvec::MyVector<TypeParam> cmv(src.begin(), src.end());
	const std::vector<TypeParam> cv(src.begin(), src.end());
	
	TestSizeValVector(cmv, size, val);
	TestSizeValVector(cv, size, val);
}

// Ctor InputIter from Size 37 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InputIter_Size42___) 
{
	std::vector<TypeParam> src = this->std_vector_size42();

	myvec::MyVector<TypeParam> mv(src.begin(), src.end());
	std::vector<TypeParam> v(src.begin(), src.end());

	TestSizeValVector(mv, src.begin(), src.end());
	TestSizeValVector(v, src.begin(), src.end());
	
	const myvec::MyVector<TypeParam> cmv(src.begin(), src.end());
	const std::vector<TypeParam> cv(src.begin(), src.end());
	
	TestSizeValVector(cmv, src.begin(), src.end());
	TestSizeValVector(cv, src.begin(), src.end());
}

// Ctor Initializer list Size Zero //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InitializerList_SizeZero___) 
{
	std::initializer_list<TypeParam> init_list = {};

	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	const myvec::MyVector<TypeParam> cmv {init_list};
	const std::vector<TypeParam> cv {init_list};
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Initializer list Size Single //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InitializerList_SizeSingle___) 
{

	std::initializer_list<TypeParam> init_list = { this->value(73) };

	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	const myvec::MyVector<TypeParam> cmv {init_list};
	const std::vector<TypeParam> cv {init_list};
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Initializer list Size 7 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InitializerList_Size7___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();

	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	const myvec::MyVector<TypeParam> cmv {init_list};
	const std::vector<TypeParam> cv {init_list};
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Copy from Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Copy_Empty___) 
{
	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;

	myvec::MyVector<TypeParam> mv(msrc);
	std::vector<TypeParam> v(src);

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	const myvec::MyVector<TypeParam> cmv(msrc);
	const std::vector<TypeParam> cv(src);
	
	TestSizeValVector(cmv);
	TestSizeValVector(cv);
}

// Ctor Copy from Single //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Copy_Single___) 
{
	TypeParam val = this->value(42);
	std::initializer_list<TypeParam> init_list = {val};
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	myvec::MyVector<TypeParam> mv(msrc);
	std::vector<TypeParam> v(src);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	const myvec::MyVector<TypeParam> cmv(msrc);
	const std::vector<TypeParam> cv(src);
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Copy from Many //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Copy_Many___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	myvec::MyVector<TypeParam> mv(msrc);
	std::vector<TypeParam> v(src);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	const myvec::MyVector<TypeParam> cmv(msrc);
	const std::vector<TypeParam> cv(src);
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Move from Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Move_Empty___) 
{
	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;

	myvec::MyVector<TypeParam> mv(std::move(msrc));
	std::vector<TypeParam> v(std::move(src));

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	const myvec::MyVector<TypeParam> cmv(std::move(msrc));
	const std::vector<TypeParam> cv(std::move(src));
	
	TestSizeValVector(cmv);
	TestSizeValVector(cv);
}

// Ctor Move from Single //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Move_Single___) 
{
	TypeParam val = this->value(42);
	std::initializer_list<TypeParam> init_list = {val};
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	myvec::MyVector<TypeParam> mv(std::move(msrc));
	std::vector<TypeParam> v(std::move(src));

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	const myvec::MyVector<TypeParam> cmv(std::move(msrc));
	const std::vector<TypeParam> cv(std::move(src));
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Move from Many //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Move_Many___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	myvec::MyVector<TypeParam> mv(std::move(msrc));
	std::vector<TypeParam> v(std::move(src));

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	const myvec::MyVector<TypeParam> cmv(std::move(msrc));
	const std::vector<TypeParam> cv(std::move(src));
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
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


// TEST(Ctor_InputIt___, Vector_String) 
// { 
// 	using T = std::string;

// 	std::vector<T> v {il_7str};

// 	myvec::MyVector<T> mv(v.begin(), v.end()); 
	
// 	EXPECT_EQ(v.size(), mv.size());
// 	EXPECT_EQ(std::distance(v.begin(), v.end()), std::distance(mv.begin(), mv.end()));
// 	EXPECT_TRUE(std::equal(v.begin(), v.end(), mv.begin()));
// 	EXPECT_STREQ(v[0].c_str(), mv[0].c_str()); // just to show, that const char* comparison is different in GTest
	
// 	myvec::MyVector<T> mv_copy(mv.begin(), mv.end());
// 	EXPECT_EQ(v.size(), mv_copy.size());
// 	EXPECT_TRUE(std::equal(v.begin(), v.end(), mv_copy.begin()));
// }

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


// TEST(Ctor_InitializerList___, Int_Multiple) 
// { 
// 	using T = int;
// 	std::initializer_list<T> inil = il_7int;

// 	myvec::MyVector<T> mv {inil};
// 	EXPECT_EQ(mv.size(), inil.size());
// 	EXPECT_GE(mv.capacity(), inil.size());
// 	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
// 	std::vector<T> v {inil};
// 	EXPECT_EQ(mv.size(), v.size());
// 	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

// }

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

// TEST(Ctor_InitializerList___, String_Multiple) 
// { 
// 	using T = std::string;
// 	std::initializer_list<T> inil = il_7str;

// 	myvec::MyVector<T> mv {inil};
// 	EXPECT_EQ(mv.size(), inil.size());
// 	EXPECT_GE(mv.capacity(), inil.size());
// 	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
// 	std::vector<T> v {inil};
// 	EXPECT_EQ(mv.size(), v.size());
// 	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

// }

// TEST(Ctor_InitializerList___, Const_Int) 
// { 
// 	using T = int;
// 	const std::initializer_list<T> inil = il_7int;

// 	const myvec::MyVector<T> mv {inil};
// 	EXPECT_EQ(mv.size(), inil.size());
// 	EXPECT_GE(mv.capacity(), inil.size());
// 	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
// 	std::vector<T> v {inil};
// 	EXPECT_EQ(mv.size(), v.size());
// 	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

// }

// TEST(Ctor_InitializerList___, Const_String) 
// { 
// 	using T = std::string;
// 	const std::initializer_list<T> inil = il_7str;

// 	const myvec::MyVector<T> mv {inil};
// 	EXPECT_EQ(mv.size(), inil.size());
// 	EXPECT_GE(mv.capacity(), inil.size());
// 	EXPECT_THAT( mv, ::testing::ElementsAreArray(inil));
	
// 	std::vector<T> v {inil};
// 	EXPECT_EQ(mv.size(), v.size());
// 	EXPECT_THAT( mv, ::testing::ElementsAreArray(v));

// }

// Move //-----------------------------------------------------------------

// TEST(Ctor_Move___, Int) 
// { 
// 	using T = int;
// 	std::initializer_list<T> inil = il_7int;

// 	myvec::MyVector<T> src_move {inil};
// 	EXPECT_EQ(src_move.size(), inil.size());
// 	EXPECT_GE(src_move.capacity(), inil.size());
// 	EXPECT_THAT( src_move, ::testing::ElementsAreArray(inil));
// 	// EXPECT_THAT( src_move, ::testing::ElementsAre(37, 43, 73)); // much worse, needs manual input
// 	// EXPECT_TRUE(std::equal(src_move.begin(), src_move.end(), inil.begin())); // I can do it
// 	// RAW LOOP: I can do it
// 	// auto comparator = 	[&inil](auto&& el)
// 	// 					{ 
// 	// 						static auto it = inil.begin(); 
// 	// 						EXPECT_EQ(el, *it);
// 	// 						++it;
// 	// 					};
// 	// std::for_each(src_move.begin(), src_move.end(), comparator);

// 	myvec::MyVector<T> move (std::move(src_move));
// 	EXPECT_EQ(move.size(), inil.size());
// 	EXPECT_GE(move.capacity(), inil.size());
// 	EXPECT_TRUE(std::equal(move.begin(), move.end(), inil.begin()));
	
// 	EXPECT_TRUE(src_move.empty());
// 	EXPECT_EQ(src_move.size(), 0);
// 	EXPECT_GE(src_move.capacity(), 0);
// 	EXPECT_EQ(src_move.data(), nullptr);

// }

// TEST(Ctor_Copy___, Move_Const_Int) 
// { 
// 	using T = int;
// 	const std::initializer_list<T> inil = il_7int;

// 	const myvec::MyVector<T> src_move {inil};
// 	EXPECT_EQ(src_move.size(), inil.size());
// 	EXPECT_GE(src_move.capacity(), inil.size());
// 	EXPECT_THAT( src_move, ::testing::ElementsAreArray(inil));
	
// 	myvec::MyVector<T> move (std::move(src_move));
// 	EXPECT_EQ(move.size(), inil.size());
// 	EXPECT_GE(move.capacity(), inil.size());
// 	EXPECT_TRUE(std::equal(move.begin(), move.end(), inil.begin()));
// }


// TEST(Ctor_Move___, _String) 
// { 
// 	using T = std::string;
// 	std::initializer_list<T> inil = il_7str;

// 	myvec::MyVector<T> src_move {inil};
// 	EXPECT_EQ(src_move.size(), inil.size());
// 	EXPECT_GE(src_move.capacity(), inil.size());
// 	EXPECT_THAT( src_move, ::testing::ElementsAreArray(inil));

	
// 	myvec::MyVector<T> move (std::move(src_move));
// 	EXPECT_EQ(move.size(), inil.size());
// 	EXPECT_GE(move.capacity(), inil.size());
// 	EXPECT_TRUE(std::equal(move.begin(), move.end(), inil.begin()));
	
// 	EXPECT_TRUE(src_move.empty());
// 	EXPECT_EQ(src_move.size(), 0);
// 	EXPECT_GE(src_move.capacity(), 0);
// 	EXPECT_EQ(src_move.data(), nullptr);
// }

// Move Allocator //-----------------------------------------------------------------

		// TODO add with custom allocators


// Move Assignment //-----------------------------------------------------------------

// TEST(Assignment_Move___, Int) 
// { 
// 	using T = int;
// 	std::initializer_list<T> inil_1 = il_3int;
// 	std::initializer_list<T> inil_2 = il_7int;

// 	myvec::MyVector<T> move_1 {inil_1};
// 	EXPECT_EQ(move_1.size(), inil_1.size());
// 	EXPECT_GE(move_1.capacity(), inil_1.size());
// 	EXPECT_THAT(move_1, ::testing::ElementsAreArray(inil_1));
	
// 	myvec::MyVector<T> move_2 {inil_2};
// 	move_2 = (std::move(move_1));

// 	EXPECT_FALSE(move_1.empty());
// 	EXPECT_EQ(move_1.size(), inil_2.size());
// 	EXPECT_GE(move_1.capacity(), inil_2.size());
// 	EXPECT_TRUE(std::equal(move_1.begin(), move_1.end(), inil_2.begin()));
	
// 	EXPECT_FALSE(move_2.empty());
// 	EXPECT_EQ(move_2.size(), inil_1.size());
// 	EXPECT_GE(move_2.capacity(), inil_1.size());
// 	EXPECT_TRUE(std::equal(move_2.begin(), move_2.end(), inil_1.begin()));

// }


// TEST(Assignment_Move___, String) 
// { 
// 	using T = std::string;
// 	std::initializer_list<T> inil_1 = il_3str;
// 	std::initializer_list<T> inil_2 = il_7str;

// 	myvec::MyVector<T> move_1 {inil_1};
// 	EXPECT_EQ(move_1.size(), inil_1.size());
// 	EXPECT_GE(move_1.capacity(), inil_1.size());
// 	EXPECT_THAT(move_1, ::testing::ElementsAreArray(inil_1));
	
// 	myvec::MyVector<T> move_2 {inil_2};
// 	move_2 = (std::move(move_1));

// 	EXPECT_FALSE(move_1.empty());
// 	EXPECT_EQ(move_1.size(), inil_2.size());
// 	EXPECT_GE(move_1.capacity(), inil_2.size());
// 	EXPECT_TRUE(std::equal(move_1.begin(), move_1.end(), inil_2.begin()));
	
// 	EXPECT_FALSE(move_2.empty());
// 	EXPECT_EQ(move_2.size(), inil_1.size());
// 	EXPECT_GE(move_2.capacity(), inil_1.size());
// 	EXPECT_TRUE(std::equal(move_2.begin(), move_2.end(), inil_1.begin()));

// }

// TEST(Assignment_Move___, Self_Int) 
// { 
// 	using T = int;
// 	std::initializer_list<T> inil_1 = il_3int;

// 	myvec::MyVector<T> move_1 {inil_1};
// 	EXPECT_EQ(move_1.size(), inil_1.size());
// 	EXPECT_GE(move_1.capacity(), inil_1.size());
// 	EXPECT_THAT(move_1, ::testing::ElementsAreArray(inil_1));
	
// 	move_1 = (std::move(move_1));

// 	EXPECT_FALSE(move_1.empty());
// 	EXPECT_EQ(move_1.size(), inil_1.size());
// 	EXPECT_GE(move_1.capacity(), inil_1.size());
// 	EXPECT_TRUE(std::equal(move_1.begin(), move_1.end(), inil_1.begin()));

// }

// TEST(Assignment_Move___, Self_String) 
// { 
// 	using T = std::string;
// 	std::initializer_list<T> inil_1 = il_3str;

// 	myvec::MyVector<T> move_1 {inil_1};
// 	EXPECT_EQ(move_1.size(), inil_1.size());
// 	EXPECT_GE(move_1.capacity(), inil_1.size());
// 	EXPECT_THAT(move_1, ::testing::ElementsAreArray(inil_1));
	
// 	move_1 = (std::move(move_1));

// 	EXPECT_FALSE(move_1.empty());
// 	EXPECT_EQ(move_1.size(), inil_1.size());
// 	EXPECT_GE(move_1.capacity(), inil_1.size());
// 	EXPECT_TRUE(std::equal(move_1.begin(), move_1.end(), inil_1.begin()));

// }

// // Ctor Copy //-----------------------------------------------------------------

// TEST(Ctor_Copy___, Int) 
// { 
// 	using T = int;
// 	std::initializer_list<T> inil_1 = il_3int;

// 	myvec::MyVector<T> src_copy {inil_1};
// 	EXPECT_EQ(src_copy.size(), inil_1.size());
// 	EXPECT_GE(src_copy.capacity(), inil_1.size());
// 	EXPECT_THAT(src_copy, ::testing::ElementsAreArray(inil_1));
	
// 	myvec::MyVector<T> copy(src_copy);

// 	EXPECT_EQ(src_copy.size(), inil_1.size());
// 	EXPECT_GE(src_copy.capacity(), inil_1.size());
// 	EXPECT_THAT(src_copy, ::testing::ElementsAreArray(inil_1));

// 	EXPECT_EQ(copy.size(), inil_1.size());
// 	EXPECT_GE(copy.capacity(), inil_1.size());
// 	EXPECT_THAT(copy, ::testing::ElementsAreArray(inil_1));
// }

// TEST(Ctor_Copy___, String) 
// { 
// 	using T = std::string;
// 	std::initializer_list<T> inil_1 = il_3str;

// 	myvec::MyVector<T> src_copy {inil_1};
// 	EXPECT_EQ(src_copy.size(), inil_1.size());
// 	EXPECT_GE(src_copy.capacity(), inil_1.size());
// 	EXPECT_THAT(src_copy, ::testing::ElementsAreArray(inil_1));
	
// 	myvec::MyVector<T> copy(src_copy);

// 	EXPECT_EQ(src_copy.size(), inil_1.size());
// 	EXPECT_GE(src_copy.capacity(), inil_1.size());
// 	EXPECT_THAT(src_copy, ::testing::ElementsAreArray(inil_1));

// 	EXPECT_EQ(copy.size(), inil_1.size());
// 	EXPECT_GE(copy.capacity(), inil_1.size());
// 	EXPECT_THAT(copy, ::testing::ElementsAreArray(inil_1));
// }

// Ctor Copy Allocator //-----------------------------------------------------------------

	// TODO: add tests

// Copy Assignment //-----------------------------------------------------------------

// TEST(Assignment_Copy___, Int) 
// { 
// 	using T = int;
// 	std::initializer_list<T> inil_1 = il_3int;
// 	std::initializer_list<T> inil_2 = il_7int;

// 	myvec::MyVector<T> copy_1 {inil_1};
// 	EXPECT_EQ(copy_1.size(), inil_1.size());
// 	EXPECT_GE(copy_1.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
// 	myvec::MyVector<T> copy_2 {inil_2};
// 	EXPECT_EQ(copy_2.size(), inil_2.size());
// 	EXPECT_GE(copy_2.capacity(), inil_2.size());
// 	EXPECT_TRUE(std::equal(copy_2.begin(), copy_2.end(), inil_2.begin()));
		
// 	copy_2 = copy_1;
// 	EXPECT_EQ(copy_1.size(), inil_1.size());
// 	EXPECT_GE(copy_1.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
// 	EXPECT_EQ(copy_2.size(), inil_1.size());
// 	EXPECT_GE(copy_2.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_2, ::testing::ElementsAreArray(inil_1));
// }

// TEST(Assignment_Copy___, String) 
// { 
// 	using T = std::string;
// 	std::initializer_list<T> inil_1 = il_3str;
// 	std::initializer_list<T> inil_2 = il_7str;

// 	myvec::MyVector<T> copy_1 {inil_1};
// 	EXPECT_EQ(copy_1.size(), inil_1.size());
// 	EXPECT_GE(copy_1.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
// 	myvec::MyVector<T> copy_2 {inil_2};
// 	EXPECT_EQ(copy_2.size(), inil_2.size());
// 	EXPECT_GE(copy_2.capacity(), inil_2.size());
// 	EXPECT_TRUE(std::equal(copy_2.begin(), copy_2.end(), inil_2.begin()));
		
// 	copy_2 = copy_1;
// 	EXPECT_EQ(copy_1.size(), inil_1.size());
// 	EXPECT_GE(copy_1.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
// 	EXPECT_EQ(copy_2.size(), inil_1.size());
// 	EXPECT_GE(copy_2.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_2, ::testing::ElementsAreArray(inil_1));
// }


// TEST(Assignment_Copy___, Self_Int) 
// { 
// 	using T = int;
// 	std::initializer_list<T> inil_1 = il_3int;

// 	myvec::MyVector<T> copy_1 {inil_1};
// 	EXPECT_EQ(copy_1.size(), inil_1.size());
// 	EXPECT_GE(copy_1.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
// 	copy_1 = copy_1;
// 	EXPECT_EQ(copy_1.size(), inil_1.size());
// 	EXPECT_GE(copy_1.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));

// }

// TEST(Assignment_Copy___, Self_String) 
// { 
// 	using T = std::string;
// 	std::initializer_list<T> inil_1 = il_3str;

// 	myvec::MyVector<T> copy_1 {inil_1};
// 	EXPECT_EQ(copy_1.size(), inil_1.size());
// 	EXPECT_GE(copy_1.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
	
// 	copy_1 = copy_1;
// 	EXPECT_EQ(copy_1.size(), inil_1.size());
// 	EXPECT_GE(copy_1.capacity(), inil_1.size());
// 	EXPECT_THAT(copy_1, ::testing::ElementsAreArray(inil_1));
// }

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


// // Empty Vector //-----------------------------------------------------------------

// TEST(EmptyVector, Default_Int) 
// { RunEmptyVectorTests<int>(); }


// TEST(EmptyVector, Default_StdVecOfString) 
// { RunEmptyVectorTests<std::vector<std::string> >(); }


// // Empty Vector Allocator //-----------------------------------------------------------------

// TEST(EmptyVector, Allocator_Int) 
// {
// 	using T = int;

// 	// VALIDATE TEST ON STD LIB
// 	std::vector<T, std::allocator<T> > st;
// 	TestEmptyVector(st);

// 	// EMPTY VECTOR
// 	myvec::MyVector<T, std::allocator<T>> t;
// 	TestEmptyVector(t);

// 	// EMPTY VECTOR CONST
// 	const myvec::MyVector<T, std::allocator<T>> ct;
// 	TestEmptyVector(ct);
// }
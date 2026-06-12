#include "MyVector.hpp"

#include "MyVectorTestFixtures.hpp"
#include "MyVectorTestFunctions.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
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

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	EXPECT_EQ(mv.data(), nullptr);
	EXPECT_EQ(v.data(), nullptr);

	const myvec::MyVector<TypeParam> cmv;
	const std::vector<TypeParam> cv;
	
	TestSizeValVector(cmv);
	TestSizeValVector(cv);

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

TYPED_TEST(TT_1, Ctor_SizeMany___) 
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

TYPED_TEST(TT_1, Ctor_SizeZero_Value__) 
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
	
	TestSizeVector(cmv, size);
	TestSizeVector(cv, size);

	EXPECT_EQ(cmv.data(), nullptr);
	EXPECT_EQ(cv.data(), nullptr);

	EXPECT_EQ(cmv.capacity(), 0);
	EXPECT_EQ(cv.capacity(), 0);
}


// Ctor Size 1 Value 42 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_SizeSingle_Value__) 
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

// Ctor SizeMany Value 42 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_SizeMany_Value__) 
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

// Ctor Size Many Value 42 : Size alias Value //-----------------------------------------------------------------

TEST(Special, Ctor_SizeMany_ValueAlias___) 
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

	auto it = src.begin();
	auto end = src.end();
	compiler_optimization_barrier(it, end);
	myvec::MyVector<TypeParam> mv(it, end);
	std::vector<TypeParam> v(it, end);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	EXPECT_EQ(mv.data(), nullptr);
	EXPECT_EQ(v.data(), nullptr);
	
	compiler_optimization_barrier(it, end);
	const myvec::MyVector<TypeParam> cmv(src.begin(), src.end());
	const std::vector<TypeParam> cv(src.begin(), src.end());
	compiler_optimization_barrier(cmv, cv);
	
	TestSizeValVector(cmv);
	TestSizeValVector(cv);
	
	EXPECT_EQ(cmv.data(), nullptr);
	EXPECT_EQ(cv.data(), nullptr);
}

// Ctor InputIter from Size 1 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InputIter_SizeSingle___) 
{
	size_t size = 1;
	TypeParam val = this->value(42);
	std::vector<TypeParam> src(1, val);

	auto it = src.begin();
	auto end = src.end();
	compiler_optimization_barrier(it, end);
	myvec::MyVector<TypeParam> mv(it, end);
	std::vector<TypeParam> v(it, end);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, size, val);
	TestSizeValVector(v, size, val);
	
	compiler_optimization_barrier(it, end);
	const myvec::MyVector<TypeParam> cmv(it, end);
	const std::vector<TypeParam> cv(it, end);
	compiler_optimization_barrier(cmv, cv);
	
	TestSizeValVector(cmv, size, val);
	TestSizeValVector(cv, size, val);
}

// Ctor InputIter from Many //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InputIter_Many___) 
{
	std::vector<TypeParam> src = this->std_vector_size42();

	auto it = src.begin();
	auto end = src.end();
	compiler_optimization_barrier(it, end);
	myvec::MyVector<TypeParam> mv(it, end);
	std::vector<TypeParam> v(it, end);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, it, end);
	TestSizeValVector(v, it, end);
	
	compiler_optimization_barrier(it, end);
	const myvec::MyVector<TypeParam> cmv(it, end);
	const std::vector<TypeParam> cv(it, end);
	compiler_optimization_barrier(cmv, cv);

	TestSizeValVector(cmv, it, end);
	TestSizeValVector(cv, it, end);

	TypeParam val = this->value(42);
	std::transform(src.begin(), src.end(), src.begin(), [&val](auto el){ return el += val; });

	TestSizeValVector(mv, v.begin(), v.end());
	TestSizeValVector(cmv, cv.begin(), cv.end());
	TestSizeValVector(cmv, v.begin(), v.end());
}

// Ctor Const InputIter from Many //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Const_InputIter_Many___) 
{
	const std::vector<TypeParam> src = this->std_vector_size42();

	auto it = src.begin();
	auto end = src.end();
	compiler_optimization_barrier(it, end);
	myvec::MyVector<TypeParam> mv(it, end);
	std::vector<TypeParam> v(it, end);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, it, end);
	TestSizeValVector(v, it, end);
	
	compiler_optimization_barrier(it, end);
	const myvec::MyVector<TypeParam> cmv(it, end);
	const std::vector<TypeParam> cv(it, end);
	compiler_optimization_barrier(cmv, cv);

	TestSizeValVector(cmv, it, end);
	TestSizeValVector(cv, it, end);

}


// Ctor Copy from Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Copy_Empty___) 
{
	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv(msrc);
	std::vector<TypeParam> v(src);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	compiler_optimization_barrier(msrc, src);
	const myvec::MyVector<TypeParam> cmv(msrc);
	const std::vector<TypeParam> cv(src);
	compiler_optimization_barrier(cmv, cv);

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

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv(msrc);
	std::vector<TypeParam> v(src);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	compiler_optimization_barrier(msrc, src);
	const myvec::MyVector<TypeParam> cmv(msrc);
	const std::vector<TypeParam> cv(src);
	compiler_optimization_barrier(cmv, cv);
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Copy from Many //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Copy_Many___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv(msrc);
	std::vector<TypeParam> v(src);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	compiler_optimization_barrier(msrc, src);
	const myvec::MyVector<TypeParam> cmv(msrc);
	const std::vector<TypeParam> cv(src);
	compiler_optimization_barrier(cmv, cv);
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Move from Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Move_Empty___) 
{
	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv(std::move(msrc));
	std::vector<TypeParam> v(std::move(src));
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	compiler_optimization_barrier(msrc, src);
	const myvec::MyVector<TypeParam> cmv(std::move(msrc));
	const std::vector<TypeParam> cv(std::move(src));
	compiler_optimization_barrier(cmv, cv);

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

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv(std::move(msrc));
	std::vector<TypeParam> v(std::move(src));
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	compiler_optimization_barrier(msrc, src);
	const myvec::MyVector<TypeParam> cmv(std::move(msrc));
	const std::vector<TypeParam> cv(std::move(src));
	compiler_optimization_barrier(cmv, cv);
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Move from Many //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_Move_Many___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv(std::move(msrc));
	std::vector<TypeParam> v(std::move(src));
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	compiler_optimization_barrier(msrc, src);
	const myvec::MyVector<TypeParam> cmv(std::move(msrc));
	const std::vector<TypeParam> cv(std::move(src));
	compiler_optimization_barrier(cmv, cv);
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}


// Ctor Initializer list Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InitializerList_SizeZero___) 
{
	std::initializer_list<TypeParam> init_list = {};

	compiler_optimization_barrier(init_list);
	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	compiler_optimization_barrier(init_list);
	const myvec::MyVector<TypeParam> cmv {init_list};
	const std::vector<TypeParam> cv {init_list};
	compiler_optimization_barrier(cmv, cv);

	TestSizeValVector(cmv);
	TestSizeValVector(cv);
}

// Ctor Initializer list Size Single //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InitializerList_SizeSingle___) 
{

	std::initializer_list<TypeParam> init_list = { this->value(73) };

	compiler_optimization_barrier(init_list);
	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	compiler_optimization_barrier(init_list);
	const myvec::MyVector<TypeParam> cmv {init_list};
	const std::vector<TypeParam> cv {init_list};
	compiler_optimization_barrier(cmv, cv);
	
	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Ctor Initializer list Size 7 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InitializerList_Size7___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();

	compiler_optimization_barrier(init_list);
	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
	
	compiler_optimization_barrier(init_list);
	const myvec::MyVector<TypeParam> cmv {init_list};
	const std::vector<TypeParam> cv {init_list};
	compiler_optimization_barrier(cmv, cv);

	TestSizeValVector(cmv, init_list.begin(), init_list.end());
	TestSizeValVector(cv, init_list.begin(), init_list.end());
}

// Assign Copy from Empty  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignCopy_Empty___) 
{
	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;
	compiler_optimization_barrier(mv, v);

	mv = msrc;
	v = src;

	TestSizeValVector(mv);
	TestSizeValVector(v);
}

// Assign Copy from Single  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignCopy_Single___) 
{
	TypeParam val = this->value(42);
	const myvec::MyVector<TypeParam> msrc {val};
	const std::vector<TypeParam> src {val};

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;
	compiler_optimization_barrier(mv, v);

	mv = msrc;
	v = src;

	TestSizeValVector(mv, 1, val);
	TestSizeValVector(v, 1, val);
}


// Assign Copy from Many  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignCopy_Many___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;
	compiler_optimization_barrier(mv, v);

	mv = msrc;
	v = src;
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
}

// Assign Copy from Self  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignCopy_Self___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	
	compiler_optimization_barrier(init_list);
	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};
	compiler_optimization_barrier(mv, v);

	mv = mv;
	v = v;
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
}

// Assign Move from Empty  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignMove_Empty___) 
{
	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;
	compiler_optimization_barrier(mv, v);

	mv = std::move(msrc);
	v = std::move(src);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv);
	TestSizeValVector(v);
}

// Assign Move from Single  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignMove_Single___) 
{
	TypeParam val = this->value(42);
	const myvec::MyVector<TypeParam> msrc {val};
	const std::vector<TypeParam> src {val};

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;
	compiler_optimization_barrier(mv, v);

	mv = std::move(msrc);
	v = std::move(src);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, 1, val);
	TestSizeValVector(v, 1, val);
}


// Assign Move from Many  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignMove_Many___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	compiler_optimization_barrier(msrc, src);
	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;
	compiler_optimization_barrier(mv, v);

	mv = std::move(msrc);
	v = std::move(src);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
}

// Assign Move from Self  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignMove_Self___) 
{
	//	standard doesn't require self move assignment to work 'safely'
	//	moved-from object is valid, but unspecified

	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	myvec::MyVector<TypeParam> mv {init_list};
	compiler_optimization_barrier(init_list);

	TestSizeValVector(mv, init_list.begin(), init_list.end());

	compiler_optimization_barrier(mv);
	myvec::MyVector<TypeParam>&& refref_self = std::move(mv);
	compiler_optimization_barrier(refref_self);
	mv = refref_self;
	compiler_optimization_barrier(mv);

	TestSizeValVector(mv, init_list.begin(), init_list.end());

}

// Begin End Return  //-----------------------------------------------------------------

TYPED_TEST(TT_1, Get_Iter_ReturnType___) 
{
	using mv_iter_type 					= myvec::MyVector<TypeParam>::iterator;
	using mv_const_iter_type			= myvec::MyVector<TypeParam>::const_iterator;
	using mv_reverse_iter_type			= myvec::MyVector<TypeParam>::reverse_iterator;
	using mv_const_reverse_iter_type	= myvec::MyVector<TypeParam>::const_reverse_iterator;

	EXPECT_TRUE(std::random_access_iterator<mv_iter_type>);
	EXPECT_TRUE(std::random_access_iterator<mv_const_iter_type>);
	EXPECT_TRUE(std::random_access_iterator<mv_reverse_iter_type>);
	EXPECT_TRUE(std::random_access_iterator<mv_const_reverse_iter_type>);

	std::initializer_list<TypeParam> init_list = this->std_init_list_7();

	myvec::MyVector<TypeParam> mv {init_list};	
	const myvec::MyVector<TypeParam> cmv {init_list};	

	// Iterator
	bool check_type = std::is_same_v< decltype(mv.begin()), decltype(mv.end()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.begin()), mv_iter_type >;
	EXPECT_TRUE( check_type );	


	// Const Iterator
	check_type = std::is_same_v< decltype(cmv.begin()), decltype(cmv.end()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.begin()), mv_const_iter_type >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.cbegin()), decltype(cmv.cend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.cbegin()), mv_const_iter_type >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.cbegin()), decltype(mv.cend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.cbegin()), mv_const_iter_type >;
	EXPECT_TRUE( check_type );


	// Reverse Iterator
	check_type = std::is_same_v< decltype(mv.rbegin()), decltype(mv.rend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.rbegin()), mv_reverse_iter_type >;
	EXPECT_TRUE( check_type );

	// Const Reverse Iterator
	check_type = std::is_same_v< decltype(cmv.rbegin()), decltype(cmv.rend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.rbegin()), mv_const_reverse_iter_type >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.crbegin()), decltype(cmv.crend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.crbegin()), mv_const_reverse_iter_type >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.crbegin()), decltype(mv.crend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.crbegin()), mv_const_reverse_iter_type >;
	EXPECT_TRUE( check_type );

}


// Iterator Copy Move //-----------------------------------------------------------------

TYPED_TEST(TT_1, Iterator___) 
{
	using mv_iter_type = myvec::MyVector<TypeParam>::iterator;
	using v_iter_type = std::vector<TypeParam>::iterator;

	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	myvec::MyVector<TypeParam> mv {init_list};	
	std::vector<TypeParam> v {init_list};
	
	mv_iter_type mv_it = mv.begin();
	mv_iter_type mv_end = mv.end();
	v_iter_type v_it = v.begin();

	// COPY CTOR
	mv_iter_type mv_it_src = mv.begin();
	mv_iter_type mv_end_src = mv.end();

	compiler_optimization_barrier(mv_it_src, mv_end_src);
	mv_iter_type mv_it_copy_ctor(mv_it_src);
	mv_iter_type mv_end_copy_ctor(mv_end_src);
	compiler_optimization_barrier(mv_it_src, mv_end_src);

	EXPECT_EQ(mv_it, mv_it_src);
	EXPECT_EQ(mv_end, mv_end_src);
	EXPECT_EQ(mv_it, mv_it_copy_ctor);
	EXPECT_EQ(mv_end, mv_end_copy_ctor);
	EXPECT_TRUE(std::equal(mv_it_copy_ctor, mv_end_copy_ctor, v_it));

	// COPY ASSIGN
	mv_it_src = mv.begin();
	mv_end_src = mv.end();

	mv_iter_type mv_it_copy_assign;
	mv_iter_type mv_end_copy_assign;
	compiler_optimization_barrier(mv_it_copy_assign, mv_end_copy_assign);

	compiler_optimization_barrier(mv_it_src, mv_end_src);	
	mv_it_copy_assign = mv_it_src;
	mv_end_copy_assign = mv_end_src;
	compiler_optimization_barrier(mv_it_src, mv_end_src);	

	EXPECT_EQ(mv_it, mv_it_src);
	EXPECT_EQ(mv_end, mv_end_src);
	EXPECT_EQ(mv_it, mv_it_copy_assign);
	EXPECT_EQ(mv_end, mv_end_copy_assign);
	EXPECT_TRUE(std::equal(mv_it_copy_assign, mv_end_copy_assign, v_it));

	// MOVE CTOR
	mv_it_src = mv.begin();
	mv_end_src = mv.end();

	compiler_optimization_barrier(mv_it_src, mv_end_src);
	mv_iter_type mv_it_move_ctor(std::move(mv_it_src));
	mv_iter_type mv_end_move_ctor(std::move(mv_end_src));
	compiler_optimization_barrier(mv_it_src, mv_end_src);
	
	EXPECT_EQ(mv_it, mv_it_move_ctor);
	EXPECT_EQ(mv_end, mv_end_move_ctor);
	EXPECT_TRUE(std::equal(mv_it_move_ctor, mv_end_move_ctor, v_it));

	// MOVE ASSIGN
	mv_it_src = mv.begin();
	mv_end_src = mv.end();
	
	mv_iter_type mv_it_move_assign;
	mv_iter_type mv_end_move_assign;
	compiler_optimization_barrier(mv_it_move_assign, mv_end_move_assign);

	compiler_optimization_barrier(mv_it_src, mv_it_src);
	mv_it_move_assign = mv_it_src;
	mv_end_move_assign = mv_end_src;
	compiler_optimization_barrier(mv_it_src, mv_it_src);
	
	EXPECT_EQ(mv_it, mv_it_move_assign);
	EXPECT_EQ(mv_end, mv_end_move_assign);	
	EXPECT_TRUE(std::equal(mv_it_move_assign, mv_end_move_assign, v_it));

	EXPECT_TRUE(std::equal(mv_it, mv_end, v_it));
}



// Const_Iterator  //-----------------------------------------------------------------

TYPED_TEST(TT_1, Const_Iterator___) 
{
	using mv_iter_type = myvec::MyVector<TypeParam>::const_iterator;
	using v_iter_type = std::vector<TypeParam>::const_iterator;

	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	const myvec::MyVector<TypeParam> mv {init_list};	
	const std::vector<TypeParam> v {init_list};
	
	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());

	mv_iter_type mv_it = mv.begin();
	mv_iter_type mv_end = mv.end();
	v_iter_type v_it = v.begin();
	v_iter_type v_end = v.end();

	TestIteratorRead(v_it, v_end, mv_it, mv_end);

	mv_iter_type cmv_it = mv.cbegin();
	mv_iter_type cmv_end = mv.cend();
	v_iter_type cv_it = v.cbegin();
	v_iter_type cv_end = v.cend();	

	EXPECT_EQ(mv_it, cmv_it);
	EXPECT_EQ(mv_end, cmv_end);
	EXPECT_EQ(v_it, cv_it);
	EXPECT_EQ(v_end, cv_end);

}

// Reverse_Iterator  //-----------------------------------------------------------------

TYPED_TEST(TT_1, Reverse_Iterator___) 
{
	using mv_iter_type = myvec::MyVector<TypeParam>::reverse_iterator;
	using v_iter_type = std::vector<TypeParam>::reverse_iterator;

	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	myvec::MyVector<TypeParam> mv {init_list};	
	std::vector<TypeParam> v {init_list};
	
	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());

	mv_iter_type mv_it = mv.rbegin();
	mv_iter_type mv_end = mv.rend();
	v_iter_type v_it = v.rbegin();
	v_iter_type v_end = v.rend();

	TestIteratorRead(v_it, v_end, mv_it, mv_end);

	TypeParam val = this->value(73);
	TestIteratorWrite(v_it, v_end, mv_it, mv_end, val);
}

// Const_Reverse_Iterator  //-----------------------------------------------------------------

TYPED_TEST(TT_1, Const_Reverse_Iterator___) 
{
	using mv_iter_type = myvec::MyVector<TypeParam>::const_reverse_iterator;
	using v_iter_type = std::vector<TypeParam>::const_reverse_iterator;

	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	myvec::MyVector<TypeParam> mv {init_list};	
	std::vector<TypeParam> v {init_list};
	
	mv_iter_type mv_it = mv.crbegin();
	mv_iter_type mv_end = mv.crend();
	v_iter_type v_it = v.crbegin();
	v_iter_type v_end = v.crend();

	TestIteratorRead(v_it, v_end, mv_it, mv_end);

	const myvec::MyVector<TypeParam> cmv {init_list};	
	const std::vector<TypeParam> cv {init_list};

	mv_iter_type cmv_it = cmv.rbegin();
	mv_iter_type cmv_end = cmv.rend();
	v_iter_type cv_it = cv.rbegin();
	v_iter_type cv_end = cv.rend();

	TestIteratorRead(cv_it, cv_end, cmv_it, cmv_end);

}

// Reserve  //-----------------------------------------------------------------

TYPED_TEST(TT_1, Reserve___) 
{
	myvec::MyVector<TypeParam> mv_empty;
	TestSizeValVector(mv_empty);
	size_t cap = 42;
	mv_empty.reserve(cap);
	EXPECT_GE(mv_empty.capacity(), cap);

	size_t size = 1;
	myvec::MyVector<TypeParam> mv_size_single(size);
	TestSizeValVector(mv_size_single, size);
	size_t prev_cap = mv_size_single.capacity();
	cap = prev_cap;
	mv_size_single.reserve(cap);
	EXPECT_EQ(mv_size_single.capacity(), prev_cap);
	++cap;
	mv_size_single.reserve(cap);
	EXPECT_GE(mv_size_single.capacity(), cap);

	size = 42;
	myvec::MyVector<TypeParam> mv_size_many(size);
	TestSizeValVector(mv_size_many, size);
	prev_cap = mv_size_many.capacity();
	cap = prev_cap;
	mv_size_many.reserve(cap);
	EXPECT_EQ(mv_size_many.capacity(), prev_cap);
	++cap;
	mv_size_many.reserve(cap);
	EXPECT_GE(mv_size_many.capacity(), cap);

}

// Push_back  //-----------------------------------------------------------------

TYPED_TEST(TT_1, Push_back___) 
{
	TypeParam val_init_vector = this->value(37);
	TypeParam val_push_back = this->value(42);

	size_t size = 0;
	myvec::MyVector<TypeParam> mv_empty(size, val_init_vector);
	TestPushBack(mv_empty, val_init_vector, val_push_back);

	size = 1;
	myvec::MyVector<TypeParam> mv_size_single(size, val_init_vector);
	TestPushBack(mv_size_single, val_init_vector, val_push_back);

	size = 42;
	myvec::MyVector<TypeParam> mv_size_many(size, val_init_vector);
	TestPushBack(mv_size_many, val_init_vector, val_push_back);

}

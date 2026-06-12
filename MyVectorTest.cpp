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

// Ctor SizeMany Value 42 //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_SizeMany_Value42___) 
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

TEST(Special, Ctor_SizeMany_Value42_Alias___) 
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

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	EXPECT_EQ(mv.data(), nullptr);
	EXPECT_EQ(v.data(), nullptr);
	
	const myvec::MyVector<TypeParam> cmv(src.begin(), src.end());
	const std::vector<TypeParam> cv(src.begin(), src.end());
	
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

	myvec::MyVector<TypeParam> mv(src.begin(), src.end());
	std::vector<TypeParam> v(src.begin(), src.end());

	TestSizeValVector(mv, size, val);
	TestSizeValVector(v, size, val);
	
	const myvec::MyVector<TypeParam> cmv(src.begin(), src.end());
	const std::vector<TypeParam> cv(src.begin(), src.end());
	
	TestSizeValVector(cmv, size, val);
	TestSizeValVector(cv, size, val);
}

// Ctor InputIter from Many //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InputIter_Many___) 
{
	const std::vector<TypeParam> src = this->std_vector_size42();

	myvec::MyVector<TypeParam> mv(src.begin(), src.end());
	std::vector<TypeParam> v(src.begin(), src.end());

	TestSizeValVector(mv, src.begin(), src.end());
	TestSizeValVector(v, src.begin(), src.end());
	
	const myvec::MyVector<TypeParam> cmv(src.begin(), src.end());
	const std::vector<TypeParam> cv(src.begin(), src.end());
	
	TestSizeValVector(cmv, src.begin(), src.end());
	TestSizeValVector(cv, src.begin(), src.end());


}

// Ctor InputIter from Many Extra //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InputIter_Many_ExtraCheck___) 
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

	TypeParam val = this->value(42);
	std::transform(src.begin(), src.end(), src.begin(), [&val](auto el){ return el += val; });

	TestSizeValVector(mv, v.begin(), v.end());
	TestSizeValVector(cmv, cv.begin(), cv.end());
	TestSizeValVector(cmv, v.begin(), v.end());
	
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


// Ctor Initializer list Empty //-----------------------------------------------------------------

TYPED_TEST(TT_1, Ctor_InitializerList_SizeZero___) 
{
	std::initializer_list<TypeParam> init_list = {};

	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};

	TestSizeValVector(mv);
	TestSizeValVector(v);
	
	const myvec::MyVector<TypeParam> cmv {init_list};
	const std::vector<TypeParam> cv {init_list};
	
	TestSizeValVector(cmv);
	TestSizeValVector(cv);
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

// Assign Copy from Empty  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignCopy_Empty___) 
{
	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;

	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;

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

	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;

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

	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;

	mv = msrc;
	v = src;

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
}

// Assign Copy from Self  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignCopy_Self___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	myvec::MyVector<TypeParam> mv {init_list};
	std::vector<TypeParam> v {init_list};

	mv = mv;
	v = v;

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
}









// Assign Move from Empty  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignMove_Empty___) 
{
	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;

	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;

	mv = std::move(msrc);
	v = std::move(src);

	TestSizeValVector(mv);
	TestSizeValVector(v);
}

// Assign Move from Single  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignMove_Single___) 
{
	TypeParam val = this->value(42);
	const myvec::MyVector<TypeParam> msrc {val};
	const std::vector<TypeParam> src {val};

	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;

	mv = std::move(msrc);
	v = std::move(src);

	TestSizeValVector(mv, 1, val);
	TestSizeValVector(v, 1, val);
}


// Assign Move from Many  //-----------------------------------------------------------------

TYPED_TEST(TT_1, AssignMove_Many___) 
{
	std::initializer_list<TypeParam> init_list = this->std_init_list_7();
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};

	myvec::MyVector<TypeParam> mv;
	std::vector<TypeParam> v;

	mv = std::move(msrc);
	v = std::move(src);

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
	
	TestSizeValVector(mv, init_list.begin(), init_list.end());

	mv = std::move(mv);
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

	// Iterator
	myvec::MyVector<TypeParam> mv {init_list};	
	
	mv_iter_type mv_it;
	mv_it = mv.begin();
	mv_iter_type mv_end;
	mv_end = mv.end();

	bool check_type = std::is_same_v< decltype(mv.begin()), decltype(mv.end()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.begin()), mv_iter_type >;
	EXPECT_TRUE( check_type );	


	// Const Iterator
	const myvec::MyVector<TypeParam> cmv {init_list};	

	mv_const_iter_type cmv_it;
	cmv_it = cmv.begin();
	mv_const_iter_type cmv_end;
	cmv_end = cmv.end();

	mv_const_iter_type ccmv_it = cmv.cbegin();
	mv_const_iter_type ccmv_end = cmv.cend();

	check_type = std::is_same_v< decltype(cmv.begin()), decltype(cmv.end()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.begin()), mv_const_iter_type >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.cbegin()), decltype(cmv.cend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.cbegin()), mv_const_iter_type >;
	EXPECT_TRUE( check_type );


	// Reverse Iterator
	
	mv_reverse_iter_type rmv_it;
	rmv_it = mv.rbegin();
	mv_reverse_iter_type rmv_end;
	rmv_end = mv.rend();

	check_type = std::is_same_v< decltype(mv.rbegin()), decltype(mv.rend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.rbegin()), mv_reverse_iter_type >;
	EXPECT_TRUE( check_type );

	// Const Reverse Iterator
	
	mv_const_reverse_iter_type crmv_it; 
	crmv_it = mv.crbegin();
	mv_const_reverse_iter_type crmv_end; 
	crmv_end = mv.crend();

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
	v_iter_type v_end = v.end();

	// COPY CTOR
	mv_iter_type mv_it_src = mv.begin();
	mv_iter_type mv_end_src = mv.end();

	mv_iter_type mv_it_copy_ctor(mv_it_src);
	(void)mv_it_copy_ctor;
	mv_iter_type mv_end_copy_ctor(mv_end_src);
	(void)mv_it_copy_ctor;
	asm volatile("" ::: "memory");

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

	// Prevents reordering of memory accesses across the barrier
	// Does NOT prevent copy elision or optimization of the assignment itself
	// asm volatile("" ::: "memory");
	
	mv_it_copy_assign = mv_it_src;
	mv_end_copy_assign = mv_end_src;
	EXPECT_EQ(mv_it, mv_it_src);
	EXPECT_EQ(mv_end, mv_end_src);
	EXPECT_EQ(mv_it, mv_it_copy_assign);
	EXPECT_EQ(mv_end, mv_end_copy_assign);
	EXPECT_TRUE(std::equal(mv_it_copy_assign, mv_end_copy_assign, v_it));

	// MOVE CTOR
	mv_it_src = mv.begin();
	mv_end_src = mv.end();
	volatile mv_iter_type mv_it_move_ctor(std::move(mv_it_src));
	volatile mv_iter_type mv_end_move_ctor(std::move(mv_end_src));
	EXPECT_EQ(mv_it, mv_it_move_ctor);
	EXPECT_EQ(mv_end, mv_end_move_ctor);
	EXPECT_TRUE(std::equal(mv_it_move_ctor, mv_end_move_ctor, v_it));

	// MOVE ASSIGN
	mv_it_src = mv.begin();
	mv_end_src = mv.end();
	volatile mv_iter_type mv_it_move_assign;
	mv_it_move_assign = mv_it_src;
	volatile mv_iter_type mv_end_move_assign;
	mv_end_move_assign = mv_end_src;
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
	
	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());

	mv_iter_type mv_it = mv.crbegin();
	mv_iter_type mv_end = mv.crend();
	v_iter_type v_it = v.crbegin();
	v_iter_type v_end = v.crend();

	TestIteratorRead(v_it, v_end, mv_it, mv_end);
}

// Iterator copy  //-----------------------------------------------------------------

TYPED_TEST(TT_1, Iter_Copy___) 
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

	// Iterator
	myvec::MyVector<TypeParam> mv {init_list};	
	
	mv_iter_type mv_it;
	mv_it = mv.begin();
	mv_iter_type mv_end;
	mv_end = mv.end();

	bool check_type = std::is_same_v< decltype(mv.begin()), decltype(mv.end()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.begin()), mv_iter_type >;
	EXPECT_TRUE( check_type );	


	// Const Iterator
	const myvec::MyVector<TypeParam> cmv {init_list};	

	mv_const_iter_type cmv_it;
	cmv_it = cmv.begin();
	mv_const_iter_type cmv_end;
	cmv_end = cmv.end();

	mv_const_iter_type ccmv_it = cmv.cbegin();
	mv_const_iter_type ccmv_end = cmv.cend();

	check_type = std::is_same_v< decltype(cmv.begin()), decltype(cmv.end()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.begin()), mv_const_iter_type >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.cbegin()), decltype(cmv.cend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(cmv.cbegin()), mv_const_iter_type >;
	EXPECT_TRUE( check_type );


	// Reverse Iterator
	
	mv_reverse_iter_type rmv_it;
	rmv_it = mv.rbegin();
	mv_reverse_iter_type rmv_end;
	rmv_end = mv.rend();

	check_type = std::is_same_v< decltype(mv.rbegin()), decltype(mv.rend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.rbegin()), mv_reverse_iter_type >;
	EXPECT_TRUE( check_type );

	// Const Reverse Iterator
	
	mv_const_reverse_iter_type crmv_it; 
	crmv_it = mv.crbegin();
	mv_const_reverse_iter_type crmv_end; 
	crmv_end = mv.crend();

	check_type = std::is_same_v< decltype(mv.crbegin()), decltype(mv.crend()) >;
	EXPECT_TRUE( check_type );

	check_type = std::is_same_v< decltype(mv.crbegin()), mv_const_reverse_iter_type >;
	EXPECT_TRUE( check_type );

}

// Clear //-----------------------------------------------------------------




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
// 	TestSizeValVector(st);

// 	// EMPTY VECTOR
// 	myvec::MyVector<T, std::allocator<T>> t;
// 	TestSizeValVector(t);

// 	// EMPTY VECTOR CONST
// 	const myvec::MyVector<T, std::allocator<T>> ct;
// 	TestSizeValVector(ct);
// }
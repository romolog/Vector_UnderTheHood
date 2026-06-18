#include "MyVector.hpp"

#include "MyVectorTestAllocators.hpp"
#include "MyVectorTestClasses.hpp"
#include "MyVectorTestFixtures.hpp"
#include "MyVectorTestFunctions.hpp"

#include <algorithm>
#include <concepts>
#include <gtest/gtest.h>
#include <gmock/gmock.h> 
#include <numeric>
#include <random>
#include <string>
#include <type_traits>
#include <vector>


using TestTypes = ::testing::Types	<
									int,
									std::string
									>;


TYPED_TEST_SUITE(Regular_, TestTypes);

// Ctor Default = Empty //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_Default___) 
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

TEST(Special_, Ctor_Default___Element_Ctor_Not_Called___)
{
	EXPECT_NO_THROW({ myvec::MyVector< myvec::AnyCtorAssignThrow > mv; });
	EXPECT_NO_THROW({ const myvec::MyVector< myvec::AnyCtorAssignThrow > cmv; });
	EXPECT_NO_THROW({ std::vector< myvec::AnyCtorAssignThrow > v; });
	EXPECT_NO_THROW({ const std::vector< myvec::AnyCtorAssignThrow > cv; });	
}

TEST(Special_, Ctor_Default___No_Heap_Allocation___)
{
	using Alloc = myvec::AllocCount<std::string>;
	Alloc alloc_counter; 
	alloc_counter.reset();
	myvec::MyVector<std::string, Alloc> mv(alloc_counter);
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	const myvec::MyVector<std::string, Alloc> cmv(alloc_counter);
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	std::vector<std::string, Alloc> v(alloc_counter);
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	const std::vector<std::string, Alloc> cv(alloc_counter);
	EXPECT_EQ(alloc_counter.allocations(), 0);
}

// Ctor Size 0 = Empty //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_SizeZero___) 
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

TEST(Special_, Ctor_SizeZero___Element_Ctor_Not_Called___)
{
	EXPECT_NO_THROW({ myvec::MyVector< myvec::AnyCtorAssignThrow > mv(0); });
	EXPECT_NO_THROW({ const myvec::MyVector< myvec::AnyCtorAssignThrow > cmv(0); });
	EXPECT_NO_THROW({ std::vector< myvec::AnyCtorAssignThrow > v(0); });
	EXPECT_NO_THROW({ const std::vector< myvec::AnyCtorAssignThrow > cv(0); });	
}

TEST(Special_, Ctor_SizeZero___No_Heap_Allocation___)
{
	using Alloc = myvec::AllocCount<std::string>;
	Alloc alloc_counter;
	alloc_counter.reset(); 
	myvec::MyVector<std::string, Alloc> mv(0, alloc_counter);
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	const myvec::MyVector<std::string, Alloc> cmv(0, alloc_counter);
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	std::vector<std::string, Alloc> v(0, alloc_counter);
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	const std::vector<std::string, Alloc> cv(0, alloc_counter);
	EXPECT_EQ(alloc_counter.allocations(), 0);
}

// Ctor Size 1 //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_SizeSingle___) 
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

TEST(Special_, Ctor_SizeSingle___Element_Ctor_Called_Times_Single___)
{
	size_t size = 1;

	myvec::CtorAssignCounter::reset();
	myvec::MyVector< myvec::CtorAssignCounter > mv(size);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::default_ctor_, size);

	myvec::CtorAssignCounter::reset();
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(size);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::default_ctor_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(size);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::default_ctor_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(size);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::default_ctor_, size);
}

// Ctor Size 42 //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_Size_Many___) 
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

TEST(Special_, Ctor_Size_Many___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;

	myvec::CtorAssignCounter::reset();
	myvec::MyVector< myvec::CtorAssignCounter > mv(size);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::default_ctor_, size);

	myvec::CtorAssignCounter::reset();
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(size);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::default_ctor_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(size);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::default_ctor_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(size);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::default_ctor_, size);

}

// Ctor Size 0 Value 42 = Empty //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_Size_Zero_InitValue__) 
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

TEST(Special_, Ctor_Size_Zero_InitValue__Element_Ctor_Not_Called___)
{
	// special Ctor called to successfully instanciate an object
	myvec::AnyCtorAssignThrow init_val (37, 73); 

	EXPECT_NO_THROW({ myvec::MyVector< myvec::AnyCtorAssignThrow > mv(0, init_val); });
	EXPECT_NO_THROW({ const myvec::MyVector< myvec::AnyCtorAssignThrow > cmv(0, init_val); });
	EXPECT_NO_THROW({ std::vector< myvec::AnyCtorAssignThrow > v(0, init_val); });
	EXPECT_NO_THROW({ const std::vector< myvec::AnyCtorAssignThrow > cv(0, init_val); });	
}

TEST(Special_, Ctor_Size_Zero_InitValue__No_Heap_Allocation___)
{
	using Alloc = myvec::AllocCount<std::string>;
	Alloc alloc_counter;
	alloc_counter.reset(); 
	myvec::MyVector<std::string, Alloc> mv(0, "init_string");
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	const myvec::MyVector<std::string, Alloc> cmv(0, "init_string");
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	std::vector<std::string, Alloc> v(0, "init_string");
	EXPECT_EQ(alloc_counter.allocations(), 0);

	alloc_counter.reset(); 
	const std::vector<std::string, Alloc> cv(0, "init_string");
	EXPECT_EQ(alloc_counter.allocations(), 0);
}

// Ctor Size 1 Value 42 //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_SizeSingle_InitValue__) 
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

TEST(Special_, Ctor_SizeSingle_InitValue__Element_Ctor_Called_Times_Size___)
{
	size_t size = 1;
	myvec::CtorAssignCounter val(37, 73);

	myvec::CtorAssignCounter::reset();
	myvec::MyVector< myvec::CtorAssignCounter > mv(size, val);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(size, val);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(size, val);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(size, val);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

}


// Ctor SizeMany Value 42 //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_SizeMany_InitValue__) 
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

TEST(Special_, Ctor_SizeMany_InitValue___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	myvec::CtorAssignCounter val(37, 73);

	myvec::CtorAssignCounter::reset();
	myvec::MyVector< myvec::CtorAssignCounter > mv(size, val);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(size, val);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(size, val);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(size, val);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
}


// Ctor Size Many Value 42 : Size alias Value //-----------------------------------------------------------------

TEST(Special_, Ctor_SizeMany_ValueAlias___) 
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

TYPED_TEST(Regular_, Ctor_InputIter_Empty___) 
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

TEST(Special_, Ctor_InputIter_Empty___Element_Ctor_Called_Times_Size___)
{

	std::vector< myvec::CtorAssignCounter > src;
	size_t size = src.size();

	auto it = src.begin();
	auto end = src.end();
	compiler_optimization_barrier(it, end);
	
	myvec::CtorAssignCounter::reset();
	myvec::MyVector< myvec::CtorAssignCounter > mv(it, end);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(it, end);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(src.begin(), src.end());
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(src.begin(), src.end());
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
}

// Ctor InputIter from Size 1 //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_InputIter_SizeSingle___) 
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

TEST(Special_, Ctor_InputIter_SizeSingle___Element_Ctor_Called_Times_Size___)
{
	size_t size = 1;
	std::vector< myvec::CtorAssignCounter > src(size, {37, 73});

	auto it = src.begin();
	auto end = src.end();
	compiler_optimization_barrier(it, end);
	
	myvec::CtorAssignCounter::reset();
	myvec::MyVector< myvec::CtorAssignCounter > mv(it, end);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(it, end);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(src.begin(), src.end());
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(src.begin(), src.end());
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
}

// Ctor InputIter from Many //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_InputIter_Many___) 
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

TEST(Special_, Ctor_InputIter_SizeMany___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	std::vector< myvec::CtorAssignCounter > src(size, {37, 73});

	auto it = src.begin();
	auto end = src.end();
	compiler_optimization_barrier(it, end);
	
	myvec::CtorAssignCounter::reset();
	myvec::MyVector< myvec::CtorAssignCounter > mv(it, end);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(it, end);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(src.begin(), src.end());
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(src.begin(), src.end());
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
}

// Ctor Const InputIter from Many //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_Const_InputIter_Many___) 
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

TYPED_TEST(Regular_, Ctor_Copy_Empty___) 
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


TEST(Special_, Ctor_Copy_Empty___Element_Ctor_Called_Times_Size___)
{
	size_t size = 0;
	const myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
	const std::vector< myvec::CtorAssignCounter > src(size, {37, 73});

	compiler_optimization_barrier(msrc, src);

	myvec::CtorAssignCounter::reset();	
	myvec::MyVector< myvec::CtorAssignCounter > mv(msrc);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(src);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();	
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(msrc);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(src);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);	

}

// Ctor Copy from Single //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_CopySingle___) 
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

TEST(Special_, Ctor_CopySingle___Element_Ctor_Called_Times_Size___)
{
	size_t size = 1;
	const myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
	const std::vector< myvec::CtorAssignCounter > src(size, {37, 73});

	compiler_optimization_barrier(msrc, src);

	myvec::CtorAssignCounter::reset();	
	myvec::MyVector< myvec::CtorAssignCounter > mv(msrc);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(src);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();	
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(msrc);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(src);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);	

}

// Ctor Copy from Many //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_CopyMany___) 
{
	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};


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

TEST(Special_, Ctor_CopyMany___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	const myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
	const std::vector< myvec::CtorAssignCounter > src(size, {37, 73});

	compiler_optimization_barrier(msrc, src);

	myvec::CtorAssignCounter::reset();	
	myvec::MyVector< myvec::CtorAssignCounter > mv(msrc);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_, 0);
	EXPECT_EQ(myvec::CtorAssignCounter::move_assign_, 0);

	myvec::CtorAssignCounter::reset();
	std::vector< myvec::CtorAssignCounter > v(src);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_, 0);
	EXPECT_EQ(myvec::CtorAssignCounter::move_assign_, 0);

	myvec::CtorAssignCounter::reset();	
	const myvec::MyVector< myvec::CtorAssignCounter > cmv(msrc);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_, 0);
	EXPECT_EQ(myvec::CtorAssignCounter::move_assign_, 0);

	myvec::CtorAssignCounter::reset();
	const std::vector< myvec::CtorAssignCounter > cv(src);
	EXPECT_EQ(myvec::CtorAssignCounter::total_, size);
	EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_, 0);
	EXPECT_EQ(myvec::CtorAssignCounter::move_assign_, 0);	
}

// Ctor Move from Empty //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_MoveEmpty___) 
{
	{
		const myvec::MyVector<TypeParam> msrc;
		const std::vector<TypeParam> src;

		compiler_optimization_barrier(msrc, src);
		myvec::MyVector<TypeParam> mv(std::move(msrc));
		std::vector<TypeParam> v(std::move(src));
		compiler_optimization_barrier(mv, v);

		TestSizeValVector(mv);
		TestSizeValVector(v);
	}
	
	{
		const myvec::MyVector<TypeParam> msrc;
		const std::vector<TypeParam> src;

		compiler_optimization_barrier(msrc, src);
		const myvec::MyVector<TypeParam> cmv(std::move(msrc));
		const std::vector<TypeParam> cv(std::move(src));
		compiler_optimization_barrier(cmv, cv);
	
		TestSizeValVector(cmv);
		TestSizeValVector(cv);
	}
}

TEST(Special_, Ctor_MoveEmpty___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	size_t expected = 0;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
	
		myvec::CtorAssignCounter::reset();	
		myvec::MyVector< myvec::CtorAssignCounter > mv(std::move(msrc));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	
		myvec::CtorAssignCounter::reset();
		std::vector< myvec::CtorAssignCounter > v(std::move(src));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	}

	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});

		compiler_optimization_barrier(msrc, src);

		myvec::CtorAssignCounter::reset();	
		const myvec::MyVector< myvec::CtorAssignCounter > cmv(std::move(msrc));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	
		myvec::CtorAssignCounter::reset();
		const std::vector< myvec::CtorAssignCounter > cv(std::move(src));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	}
}

// Ctor Move from Single //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_MoveSingle___) 
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

TEST(Special_, Ctor_MoveSingle___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	size_t expected = 0;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
	
		myvec::CtorAssignCounter::reset();	
		myvec::MyVector< myvec::CtorAssignCounter > mv(std::move(msrc));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	
		myvec::CtorAssignCounter::reset();
		std::vector< myvec::CtorAssignCounter > v(std::move(src));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	}

	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});

		compiler_optimization_barrier(msrc, src);

		myvec::CtorAssignCounter::reset();	
		const myvec::MyVector< myvec::CtorAssignCounter > cmv(std::move(msrc));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	
		myvec::CtorAssignCounter::reset();
		const std::vector< myvec::CtorAssignCounter > cv(std::move(src));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	}
}


// Ctor Move from Many //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_Move_Many___) 
{
	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};

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

TEST(Special_, Ctor_MoveMany___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	size_t expected = 0;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
	
		myvec::CtorAssignCounter::reset();	
		myvec::MyVector< myvec::CtorAssignCounter > mv(std::move(msrc));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	
		myvec::CtorAssignCounter::reset();
		std::vector< myvec::CtorAssignCounter > v(std::move(src));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	}

	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});

		compiler_optimization_barrier(msrc, src);

		myvec::CtorAssignCounter::reset();	
		const myvec::MyVector< myvec::CtorAssignCounter > cmv(std::move(msrc));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	
		myvec::CtorAssignCounter::reset();
		const std::vector< myvec::CtorAssignCounter > cv(std::move(src));
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	}
}

// Ctor Initializer list Empty //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_InitializerList_SizeZero___) 
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

TEST(Special_, Ctor_InitializerList_SizeZero___Element_Ctor_Called_Times_Size___)
{
	size_t expected = 0;

	std::initializer_list< myvec::CtorAssignCounter > init_list = {};
	compiler_optimization_barrier(init_list);
	{
		myvec::CtorAssignCounter::reset();	
		myvec::MyVector< myvec::CtorAssignCounter > mv {init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	
		myvec::CtorAssignCounter::reset();
		std::vector< myvec::CtorAssignCounter > v {init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	}

	{
		myvec::CtorAssignCounter::reset();	
		const myvec::MyVector< myvec::CtorAssignCounter > cmv{init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	
		myvec::CtorAssignCounter::reset();
		const std::vector< myvec::CtorAssignCounter > cv{init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected);
	}
}


// Ctor Initializer list Size Single //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_InitializerList_SizeSingle___) 
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

TEST(Special_, Ctor_InitializerList_SizeSingle___Element_Ctor_Called_Times_Size___)
{
	std::initializer_list< myvec::CtorAssignCounter > init_list = { myvec::CtorAssignCounter(37, 73) };
	size_t expected_ttl = 1;
	size_t expected_move = 0;
	size_t expected_copy = 1;
	compiler_optimization_barrier(init_list);

	{
		myvec::CtorAssignCounter::reset();	
		myvec::MyVector< myvec::CtorAssignCounter > mv {init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_ + myvec::CtorAssignCounter::move_assign_, expected_move);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_ctor_ + myvec::CtorAssignCounter::copy_assign_, expected_copy);

		myvec::CtorAssignCounter::reset();
		std::vector< myvec::CtorAssignCounter > v {init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_ + myvec::CtorAssignCounter::move_assign_, expected_move);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_ctor_ + myvec::CtorAssignCounter::copy_assign_, expected_copy);
	}

	{
		myvec::CtorAssignCounter::reset();	
		const myvec::MyVector< myvec::CtorAssignCounter > cmv{init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_ + myvec::CtorAssignCounter::move_assign_, expected_move);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_ctor_ + myvec::CtorAssignCounter::copy_assign_, expected_copy);
	
		myvec::CtorAssignCounter::reset();
		const std::vector< myvec::CtorAssignCounter > cv{init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_ + myvec::CtorAssignCounter::move_assign_, expected_move);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_ctor_ + myvec::CtorAssignCounter::copy_assign_, expected_copy);
	}
}


// Ctor Initializer list Size 7 //-----------------------------------------------------------------

TYPED_TEST(Regular_, Ctor_InitializerList_SizeMany___) 
{
	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};

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

TEST(Special_, Ctor_InitializerList_SizeMany___Element_Ctor_Called_Times_Size___)
{
	std::initializer_list< myvec::CtorAssignCounter > init_list = 
		{ 	myvec::CtorAssignCounter(1, 42),
			myvec::CtorAssignCounter(2, 42),
			myvec::CtorAssignCounter(3, 42),
			myvec::CtorAssignCounter(4, 42),
			myvec::CtorAssignCounter(5, 42)			
		};

	size_t expected_ttl = init_list.size();
	size_t expected_move = 0;
	size_t expected_copy = init_list.size();
	compiler_optimization_barrier(init_list);

	{
		myvec::CtorAssignCounter::reset();		
		myvec::MyVector< myvec::CtorAssignCounter > mv {init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_ + myvec::CtorAssignCounter::move_assign_, expected_move);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_ctor_ + myvec::CtorAssignCounter::copy_assign_, expected_copy);

		myvec::CtorAssignCounter::reset();
		std::vector< myvec::CtorAssignCounter > v {init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_ + myvec::CtorAssignCounter::move_assign_, expected_move);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_ctor_ + myvec::CtorAssignCounter::copy_assign_, expected_copy);
	}

	{
		myvec::CtorAssignCounter::reset();	
		const myvec::MyVector< myvec::CtorAssignCounter > cmv{init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_ + myvec::CtorAssignCounter::move_assign_, expected_move);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_ctor_ + myvec::CtorAssignCounter::copy_assign_, expected_copy);
	
		myvec::CtorAssignCounter::reset();
		const std::vector< myvec::CtorAssignCounter > cv{init_list};
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::move_ctor_ + myvec::CtorAssignCounter::move_assign_, expected_move);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_ctor_ + myvec::CtorAssignCounter::copy_assign_, expected_copy);
	}
}

// Assign Copy from Empty  //-----------------------------------------------------------------

TYPED_TEST(Regular_, AssignCopy_Empty_to_Empty___) 
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


TEST(Special_, AssignCopy_Empty_to_Empty___Element_Ctor_Called_Times_Size___)
{
	size_t size = 0;
	size_t expected_ttl = size;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
		myvec::MyVector< myvec::CtorAssignCounter > mv;
		std::vector< myvec::CtorAssignCounter > v;
		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = msrc;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		
		myvec::CtorAssignCounter::reset();
		v = src;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	}
}


TYPED_TEST(Regular_, AssignCopy_Empty_to_Many___) 
{
	size_t init_size = 42;
	TypeParam init_val = this->value(42);

	const myvec::MyVector<TypeParam> msrc;
	const std::vector<TypeParam> src;
	compiler_optimization_barrier(msrc, src);

	myvec::MyVector<TypeParam> mv(init_size, init_val);
	std::vector<TypeParam> v(init_size, init_val);
	compiler_optimization_barrier(mv, v);

	mv = msrc;
	v = src;

	TestSizeValVector(mv);
	TestSizeValVector(v);
}

TEST(Special_, AssignCopy_Empty_to_Many___Element_Ctor_Called_Times_Size___)
{
	size_t init_size = 42;

	size_t size = 0;
	size_t expected_ttl = size;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
		compiler_optimization_barrier(msrc, src);
		
		myvec::CtorAssignCounter::reset();
		myvec::MyVector< myvec::CtorAssignCounter > mv(init_size, {37, 73});
		EXPECT_EQ(myvec::CtorAssignCounter::created_, init_size);

		myvec::CtorAssignCounter::reset();
		std::vector< myvec::CtorAssignCounter > v(init_size, {37, 73});
		EXPECT_EQ(myvec::CtorAssignCounter::created_, init_size);

		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = msrc;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::dtor_, init_size);
		
		myvec::CtorAssignCounter::reset();
		v = src;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::dtor_, init_size);		
	}
}

// Assign Copy from Single  //-----------------------------------------------------------------

TYPED_TEST(Regular_, AssignCopy_Single_to_Empty___) 
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

TEST(Special_, AssignCopy_Single_to_Empty___Element_Ctor_Called_Times_Size___)
{
	size_t size = 1;
	size_t expected_ttl = size;
	size_t expected_copy = size;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
		myvec::MyVector< myvec::CtorAssignCounter > mv;
		std::vector< myvec::CtorAssignCounter > v;
		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = msrc;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_assign_ + myvec::CtorAssignCounter::copy_ctor_, expected_copy);
		
		myvec::CtorAssignCounter::reset();
		v = src;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_assign_ + myvec::CtorAssignCounter::copy_ctor_, expected_copy);
	}
}

// Assign Copy from Many  //-----------------------------------------------------------------

TYPED_TEST(Regular_, AssignCopy_Many_to_Empty___) 
{
	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
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

TEST(Special_, AssignCopy_Many_to_Empty___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	size_t expected_ttl = size;
	size_t expected_copy = size;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
		myvec::MyVector< myvec::CtorAssignCounter > mv;
		std::vector< myvec::CtorAssignCounter > v;
		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = msrc;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_assign_ + myvec::CtorAssignCounter::copy_ctor_, expected_copy);
		
		myvec::CtorAssignCounter::reset();
		v = src;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_assign_ + myvec::CtorAssignCounter::copy_ctor_, expected_copy);
	}
}

TYPED_TEST(Regular_, AssignCopy_Many_to_Many___) 
{
	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
	
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};
	compiler_optimization_barrier(msrc, src);

	myvec::MyVector<TypeParam> mv(42, this->value(42));
	std::vector<TypeParam> v(42, this->value(42));
	compiler_optimization_barrier(mv, v);

	mv = msrc;
	v = src;
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
}

TEST(Special_, AssignCopy_Many_to_Many___Element_Ctor_Called_Times_Size___)
{
	size_t init_size = 37;

	size_t size = 42;
	size_t expected_ttl = size;
	size_t expected_copy = size;
	
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
		compiler_optimization_barrier(msrc, src);
	
		myvec::CtorAssignCounter::reset();	
		myvec::MyVector< myvec::CtorAssignCounter > mv(init_size, {37, 73});
		EXPECT_EQ(myvec::CtorAssignCounter::created_, init_size);

		myvec::CtorAssignCounter::reset();	
		std::vector< myvec::CtorAssignCounter > v(init_size, {37, 73});
		EXPECT_EQ(myvec::CtorAssignCounter::created_, init_size);		
		
		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = msrc;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_assign_ + myvec::CtorAssignCounter::copy_ctor_, expected_copy);
		EXPECT_EQ(myvec::CtorAssignCounter::dtor_, init_size);
		
		myvec::CtorAssignCounter::reset();
		v = src;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_assign_ + myvec::CtorAssignCounter::copy_ctor_, expected_copy);
		EXPECT_EQ(myvec::CtorAssignCounter::dtor_, init_size);
	}
}

// Assign Copy from Self  //-----------------------------------------------------------------

TYPED_TEST(Regular_, AssignCopy_Self___) 
{
	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
	
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

TEST(Special_, AssignCopy_Self___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	size_t expected_ttl = 0;
	size_t expected_copy = 0;
	{
		myvec::MyVector< myvec::CtorAssignCounter > mv(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > v(size, {37, 73});
	
		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = mv;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_assign_ + myvec::CtorAssignCounter::copy_ctor_, expected_copy);
		
		myvec::CtorAssignCounter::reset();
		v = v;
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
		EXPECT_EQ(myvec::CtorAssignCounter::copy_assign_ + myvec::CtorAssignCounter::copy_ctor_, expected_copy);
	}
}

// Assign Move from Empty  //-----------------------------------------------------------------

TYPED_TEST(Regular_, AssignMove_Empty_to_Empty___) 
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

TEST(Special_, AssignMove_Empty_to_Empty___Element_Ctor_Called_Times_Size___)
{
	size_t size = 0;
	size_t expected_ttl = 0;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
		myvec::MyVector< myvec::CtorAssignCounter > mv;
		std::vector< myvec::CtorAssignCounter > v;
		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = std::move(msrc);
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	
		myvec::CtorAssignCounter::reset();
		v = std::move(src);
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	}
}


// Assign Move from Single  //-----------------------------------------------------------------

TYPED_TEST(Regular_, AssignMove_Single_to_Empty___) 
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


TEST(Special_, AssignMove_Single_to_Empty___Element_Ctor_Called_Times_Size___)
{
	size_t size = 1;
	size_t expected_ttl = 0;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
		myvec::MyVector< myvec::CtorAssignCounter > mv;
		std::vector< myvec::CtorAssignCounter > v;
		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = std::move(msrc);
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	
		myvec::CtorAssignCounter::reset();
		v = std::move(src);
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	}
}

// Assign Move from Many  //-----------------------------------------------------------------

TYPED_TEST(Regular_, AssignMove_Many_to_Empty___) 
{
	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
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

TEST(Special_, AssignMove_Many_to_Empty___Element_Ctor_Called_Times_Size___)
{
	size_t size = 42;
	size_t expected_ttl = 0;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
	
		compiler_optimization_barrier(msrc, src);
		myvec::MyVector< myvec::CtorAssignCounter > mv;
		std::vector< myvec::CtorAssignCounter > v;
		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = std::move(msrc);
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	
		myvec::CtorAssignCounter::reset();
		v = std::move(src);
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	}
}

TYPED_TEST(Regular_, AssignMove_Many_to_Many___) 
{
	
	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
	const myvec::MyVector<TypeParam> msrc {init_list};
	const std::vector<TypeParam> src {init_list};
	compiler_optimization_barrier(msrc, src);
	
	size_t size = 42;
	myvec::MyVector<TypeParam> mv(size, this->value(42));
	std::vector<TypeParam> v(size, this->value(42));
	compiler_optimization_barrier(mv, v);

	mv = std::move(msrc);
	v = std::move(src);
	compiler_optimization_barrier(mv, v);

	TestSizeValVector(mv, init_list.begin(), init_list.end());
	TestSizeValVector(v, init_list.begin(), init_list.end());
}

TEST(Special_, AssignMove_Many_to_Many___Element_Ctor_Called_Times_Size___)
{
	size_t init_size = 37;

	size_t size = 42;
	size_t expected_ttl = 0;
	{
		myvec::MyVector< myvec::CtorAssignCounter > msrc(size, {37, 73});
		std::vector< myvec::CtorAssignCounter > src(size, {37, 73});
		compiler_optimization_barrier(msrc, src);
	
		myvec::CtorAssignCounter::reset();
		myvec::MyVector< myvec::CtorAssignCounter > mv(init_size, {37, 73});
		EXPECT_EQ(myvec::CtorAssignCounter::created_, init_size);

		myvec::CtorAssignCounter::reset();
		std::vector< myvec::CtorAssignCounter > v(init_size, {37, 73});
		EXPECT_EQ(myvec::CtorAssignCounter::created_, init_size);

		compiler_optimization_barrier(mv, v);

		myvec::CtorAssignCounter::reset();	
		mv = std::move(msrc);
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	
		myvec::CtorAssignCounter::reset();
		v = std::move(src);
		EXPECT_EQ(myvec::CtorAssignCounter::total_, expected_ttl);
	}
}	


// Assign Move from Self  //-----------------------------------------------------------------

TYPED_TEST(Regular_, AssignMove_Self___) 
{
	//	standard doesn't require self move assignment to work 'safely'
	//	moved-from object is valid, but unspecified

	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
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

TYPED_TEST(Regular_, Iter_ReturnType___) 
{
	using mv_iter_type 					= myvec::MyVector<TypeParam>::iterator;
	using mv_const_iter_type			= myvec::MyVector<TypeParam>::const_iterator;
	using mv_reverse_iter_type			= myvec::MyVector<TypeParam>::reverse_iterator;
	using mv_const_reverse_iter_type	= myvec::MyVector<TypeParam>::const_reverse_iterator;

	EXPECT_TRUE(std::random_access_iterator<mv_iter_type>);
	EXPECT_TRUE(std::random_access_iterator<mv_const_iter_type>);
	EXPECT_TRUE(std::random_access_iterator<mv_reverse_iter_type>);
	EXPECT_TRUE(std::random_access_iterator<mv_const_reverse_iter_type>);

	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};

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

TYPED_TEST(Regular_, Iterator___) 
{
	using mv_iter_type = myvec::MyVector<TypeParam>::iterator;
	using v_iter_type = std::vector<TypeParam>::iterator;

	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
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

TYPED_TEST(Regular_, Const_Iterator___) 
{
	using mv_iter_type = myvec::MyVector<TypeParam>::const_iterator;
	using v_iter_type = std::vector<TypeParam>::const_iterator;

	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
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

TYPED_TEST(Regular_, Reverse_Iterator___) 
{
	using mv_iter_type = myvec::MyVector<TypeParam>::reverse_iterator;
	using v_iter_type = std::vector<TypeParam>::reverse_iterator;

	std::initializer_list<TypeParam> init_list	{	this->value(1),
													this->value(2),
													this->value(3),
													this->value(4),
													this->value(5),
													this->value(6),
													this->value(7),
													this->value(8)
												};
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

TYPED_TEST(Regular_, Const_Reverse_Iterator___) 
{
	using mv_iter_type = myvec::MyVector<TypeParam>::const_reverse_iterator;
	using v_iter_type = std::vector<TypeParam>::const_reverse_iterator;

	std::initializer_list<TypeParam> init_list {	this->value(1), 
													this->value(2), 
													this->value(3), 
													this->value(4), 
													this->value(5), 
													this->value(6), 
													this->value(7), 
													this->value(8)
												};

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

TYPED_TEST(Regular_, Reserve___) 
{
	myvec::MyVector<TypeParam> mv_empty;
	TestReserve(mv_empty);

	size_t size = 1;
	myvec::MyVector<TypeParam> mv_size_single(size);
	TestReserve(mv_size_single);

	size = 42;
	myvec::MyVector<TypeParam> mv_size_many(size);
	TestReserve(mv_size_many);
}

TEST(Special_, Reallocation___Element_NoMove___)
{
	using TypeParam = myvec::NoMove;

	myvec::MyVector<TypeParam> mv_empty;
	TestSizeValVector(mv_empty);
	TestReserve(mv_empty);

	size_t size = 1;
	myvec::MyVector<TypeParam> mv_size_single(size);
	TestSizeValVector(mv_size_single, size);
	TestReserve(mv_size_single);

	size = 42;
	myvec::MyVector<TypeParam> mv_size_many(size);
	TestSizeValVector(mv_size_many, size);
	TestReserve(mv_size_single);
}

TEST(Special_, Reallocation___Element_NoCopy___)
{
	using TypeParam = myvec::NoCopy;

	myvec::MyVector<TypeParam> mv_empty;
	TestSizeValVector(mv_empty);
	TestReserve(mv_empty);
	

	size_t size = 1;
	myvec::MyVector<TypeParam> mv_size_single(size);
	TestSizeValVector(mv_size_single, size);
	TestReserve(mv_size_single);

	size = 42;
	myvec::MyVector<TypeParam> mv_size_many(size);
	TestSizeValVector(mv_size_many, size);
	TestReserve(mv_size_single);
}

TEST(Special_, Reallocation___Element_CtorAssignCounter___)
{
	using T = myvec::CtorAssignCounter;

	size_t size = 42;
	int start_val = 1;
	myvec::MyVector<T> mv = make_my_vector_size<T>(size, start_val);
	std::vector<T> v = make_std_vector_size<T>(size, start_val);
	
	size_t expected_move = size;
	size_t expected_ttl = expected_move;
	{
		T* prev_data = v.data();
		T::reset();
		mv.reserve(mv.capacity() + 42);
		for (size_t i = 0; prev_data == mv.data() && i < 100; ++i)
			mv.reserve(mv.capacity() + 42);
		EXPECT_NE(mv.data(), prev_data) << "Reallocations happend to the same memory 100 times in a row";
		EXPECT_EQ(T::move_assign_ + T::move_ctor_, expected_move);
		EXPECT_EQ(T::total_, expected_ttl);
	}

	{
		auto prev_data = v.data();
		T::reset();
		mv.reserve(mv.capacity() + 42);
		for (size_t i = 0; prev_data == mv.data() && i < 100; ++i)
			mv.reserve(mv.capacity() + 42);
		EXPECT_NE(mv.data(), prev_data) << "Reallocations happend to the same memory 100 times in a row";
		EXPECT_EQ(T::move_assign_ + T::move_ctor_, expected_move);
		EXPECT_EQ(T::total_, expected_ttl);
	}

}

TEST(Special_, Reserve___Exception_Safety___ElementMoveThrow___) 
{
	myvec::StructThrow::copies_moves_before_throw_ = 7;
	using T = myvec::StructThrow; // count move/copy, throws at 7th move/copy


	size_t size = 5;
	myvec::MyVector<T> mv(size, T()); // 5 copies made 
	ASSERT_EQ(mv.capacity(), size); // for push_back reallocation is needed
	
	auto prev_data = mv.data();
	auto prev_cap = mv.capacity();
	auto prev_size = mv.size();

	// each object of StructThrow has its unique key, 
	//		which is not copyable or moveable
	//		can be read by 'key()' method
	// copy all 'keys' into std::vector
	std::vector<int> prev_vals(prev_size);
	auto it_mv = mv.begin();
	std::generate(prev_vals.begin(), prev_vals.end(), [&it_mv]()
													{ 
														int key = (*it_mv).key();
														++it_mv;
														return key;
													});

	// at reallocation: 6th move/copy successful, 7th throws
	ASSERT_THROW( mv.reserve(prev_cap + 42), myvec::StructThrow::Exception ); 

	// check exception guaranty: vector remains valid on reallocation fail
	//		check data ptr, capacity, size
	EXPECT_EQ(mv.data(), prev_data);
	EXPECT_EQ(mv.capacity(), prev_cap);
	EXPECT_EQ(mv.size(), prev_size);
	// 		check that all 'keys' remain the same ('keys' are not copyable or moveable)
	auto it_v = prev_vals.begin();
	std::for_each(mv.begin(), mv.end(), [&it_v](auto&& el)
										{
											EXPECT_EQ(el.key(), *it_v);
											++it_v;
										});
}

TEST(Special_, Reserve___Exception_Safety___ReallocationThrow___)
{
	size_t size = 42;
	myvec::AllocThrow<int> throwing_alloc;
	myvec::AllocThrow<int>::allocs_before_throw_ = 2; // allows to create a vector and its copy
	
	try
	{
		// allocs_before_throw = 2;
		myvec::MyVector<int, myvec::AllocThrow<int> > mv(size, throwing_alloc);
		// allocs_before_throw = 1;

		std::iota(mv.begin(), mv.end(), 1);
		ASSERT_EQ(mv.capacity(), size);
		
		auto prev_data = mv.data();
		auto prev_cap = mv.capacity();
		auto prev_size = mv.size();
		auto copy(mv); 
		// allocs_before_throw = 0;
	
		ASSERT_THROW( mv.reserve(prev_cap + 42), std::bad_alloc); // AllocThrow will throw at reallocation
	
		EXPECT_EQ(mv.data(), prev_data);
		EXPECT_EQ(mv.capacity(), prev_cap);
		EXPECT_EQ(mv.size(), prev_size);
		EXPECT_TRUE(std::equal(mv.begin(), mv.end(), copy.begin()));
	}
	catch(const std::bad_alloc& ba)
	{
		FAIL() << "\n\tCaught bad_alloc at init vec\n\n";
	}
	
}

// Push_back  //-----------------------------------------------------------------

TYPED_TEST(Regular_, Push_back___) 
{
	int val_start = 0;
	TypeParam val_push_back = this->value(val_start);
	
	++val_start; // =1
	TypeParam val_init = this->value(val_start);


	size_t size = 0;
	myvec::MyVector<TypeParam> mv_empty(size, val_init);
	TestPushBack(mv_empty, val_start, val_push_back);
	mv_empty.reserve(mv_empty.capacity() + 42);
	TestPushBack(mv_empty, val_start, val_push_back);	

	size = 1;
	myvec::MyVector<TypeParam> mv_size_single(size, val_init);
	TestPushBack(mv_size_single, val_start, val_push_back);
	mv_size_single.reserve(mv_size_single.capacity() + 42);
	TestPushBack(mv_size_single, val_start, val_push_back);

	size = 42;
	myvec::MyVector<TypeParam> mv_size_many(this->make_vector_size(size, val_start));
	val_start += size; // = 43
	TestPushBack(mv_size_many, val_start, val_push_back);
	mv_size_many.reserve(mv_size_many.capacity() + 42);
	TestPushBack(mv_size_many, val_start, val_push_back);

}

TEST(Special_, Push_back___ExceptionSafety___ElementMoveThrow___) 
{
	myvec::StructThrow::copies_moves_before_throw_ = 7;
	using T = myvec::StructThrow; 

	size_t size = 5;
	myvec::MyVector<T> mv(size, T()); 
	ASSERT_EQ(mv.capacity(), size); 
	
	auto prev_data = mv.data();
	auto prev_cap = mv.capacity();
	auto prev_size = mv.size();

	std::vector<int> prev_vals(prev_size);
	auto it_mv = mv.begin();
	std::generate(prev_vals.begin(), prev_vals.end(), [&it_mv]()
													{ 
														int key = (*it_mv).key();
														++it_mv;
														return key;
													});

	ASSERT_THROW( mv.push_back(T()), myvec::StructThrow::Exception );
	
	EXPECT_EQ(mv.data(), prev_data);
	EXPECT_EQ(mv.capacity(), prev_cap);
	EXPECT_EQ(mv.size(), prev_size);
	auto it_v = prev_vals.begin();
	std::for_each(mv.begin(), mv.end(), [&it_v](auto&& el)
										{
											EXPECT_EQ(el.key(), *it_v);
											++it_v;
										});
}


TEST(Special_, Push_back___Exception_Safety___ReallocationThrow___)
{
	size_t size = 42;
	myvec::AllocThrow<int> throwing_alloc;
	myvec::AllocThrow<int>::allocs_before_throw_ = 2; // allows to create a vector and its copy
	
	try
	{
		// allocs_before_throw = 2;
		myvec::MyVector<int, myvec::AllocThrow<int> > mv(size, throwing_alloc);
		// allocs_before_throw = 1;

		std::iota(mv.begin(), mv.end(), 1);
		ASSERT_EQ(mv.capacity(), size); // push_back needs reallocation
		
		auto prev_data = mv.data();
		auto prev_cap = mv.capacity();
		auto prev_size = mv.size();
		auto copy(mv); 
		// allocs_before_throw = 0;
	
		ASSERT_THROW( mv.push_back(73), std::bad_alloc); // AllocThrow will throw at reallocation
	
		EXPECT_EQ(mv.data(), prev_data);
		EXPECT_EQ(mv.capacity(), prev_cap);
		EXPECT_EQ(mv.size(), prev_size);
		EXPECT_TRUE(std::equal(mv.begin(), mv.end(), copy.begin()));
	}
	catch(const std::bad_alloc& ba)
	{
		FAIL() << "\n\tCaught bad_alloc at init vec\n\n";
	}
	
}
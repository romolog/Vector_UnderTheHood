#include "MyVector.hpp"

#include <iterator>
#include <concepts>
#include <random>
#include <string>
#include <type_traits>
#include <vector>


template <typename T>
__attribute__((always_inline)) 
void compiler_optimization_barrier(T& obj) {
	asm volatile("" : "+m"(obj) : : "memory");
}

template <typename T, typename U>
__attribute__((always_inline)) 
void compiler_optimization_barrier(T&& obj_1, U&& obj_2) {
    using T1 = std::remove_reference_t<T>;
    using U1 = std::remove_reference_t<U>;
    
    if constexpr (std::is_const_v<T1>) {
        // Read-only barrier
        asm volatile("" : : "m"(obj_1) : "memory");
    } else {
        // Read-write barrier
        asm volatile("" : "+m"(obj_1) : : "memory");
    }
    
    if constexpr (std::is_const_v<U1>) {
        asm volatile("" : : "m"(obj_2) : "memory");
    } else {
        asm volatile("" : "+m"(obj_2) : : "memory");
    }
}

template<class Vec>
void TestSizeVector(Vec& t, size_t size)
{

	EXPECT_EQ(t.size(), size);
	EXPECT_GE(t.capacity(), size);
	
	if (size)
	{
		EXPECT_NE(t.data(), nullptr);
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
}

template<typename Vec>
void TestSizeValVector(Vec& t)
{
	TestSizeVector(t, 0);
}

template<typename Vec>
void TestSizeValVector(Vec& t, size_t size)
{
	TestSizeVector(t, size);
}

template<typename Vec>
void TestSizeValVector(Vec& t, size_t size, const typename Vec::value_type& val)
{

	TestSizeVector(t, size);

	std::for_each(t.begin(), t.end(), [val](auto&& el){EXPECT_EQ(el, val);});
	// EXPECT_EQ(t[0], val);

}

template<typename Vec, typename Iter>
	requires std::input_iterator<Iter>
void TestSizeValVector(Vec& t, Iter it, Iter end)
{

	TestSizeVector(t, std::distance(it, end));

	std::for_each(t.begin(), t.end(), [&it](auto&& el){EXPECT_EQ(el, *it++);});

}


template <typename Iter, typename MyIter>
	requires std::random_access_iterator<Iter> && std::random_access_iterator<MyIter>
void TestIteratorRead(Iter v_it, Iter v_end, MyIter mv_it, MyIter mv_end)
{
	
	ASSERT_GE(std::distance(v_it, v_end), 7);
	EXPECT_EQ(*mv_it, *v_it); // *0
	EXPECT_TRUE(std::equal(mv_it, mv_end, v_it));

	for (auto it = mv_it; it < mv_end; ++it)
	{
		size_t len = std::distance(it, mv_end);
		for (size_t i = 0; i < len; ++i)
			EXPECT_EQ(v_it[i], mv_it[i]); 
	}

	
	EXPECT_EQ(mv_it[0], v_it[0]); // *0
	EXPECT_EQ(mv_it[3], v_it[3]); // *3
	EXPECT_EQ(mv_it[3], v_it[3]); // *3		
	EXPECT_EQ(std::distance(mv_it, mv_end), std::distance(v_it, v_end));
	++mv_it; //1
	++v_it;
	EXPECT_EQ(*mv_it, *v_it);
	EXPECT_EQ(*mv_it++, *v_it++); // 2
	std::advance(mv_it, 3);	 //5
	std::advance(v_it, 3);
	EXPECT_EQ(*mv_it, *v_it);
	std::advance(mv_it, -2); //3
	std::advance(v_it, -2);
	EXPECT_EQ(*mv_it, *v_it);
	--mv_it; //2
	--v_it;
	EXPECT_EQ(*mv_it, *v_it);
	mv_it += 3; //5
	v_it += 3;
	EXPECT_EQ(*mv_it, *v_it);
	mv_it -= 2; //3
	v_it -= 2;
	EXPECT_EQ(*mv_it, *v_it);	
	EXPECT_EQ(*(mv_it + 3), *(v_it + 3)); // *6
	EXPECT_EQ(*(1 + mv_it), *(1 + v_it)); // *4	
	EXPECT_EQ(*(mv_it - 2), *(v_it - 2)); // *1
	EXPECT_EQ(mv_end - mv_it, v_end - v_it);
	EXPECT_EQ(mv_end - mv_it, std::distance(mv_it, mv_end));
	EXPECT_EQ(v_end - v_it, std::distance(v_it, v_end));

}

template <typename Iter, typename MyIter, typename Val>
	requires std::random_access_iterator<Iter> && std::random_access_iterator<MyIter>
void TestIteratorWrite(Iter v_it, Iter v_end, MyIter mv_it, MyIter mv_end, Val val)
{
	
	ASSERT_GE(std::distance(v_it, v_end), 7);

	EXPECT_EQ(*mv_it, *v_it); // *0 = 'Hello' / 1
	*mv_it += val;
	*v_it += val;
	EXPECT_EQ(*mv_it, *v_it); // *0 = 'Hello42' / 43
	++mv_it; //1
	++v_it;
	*mv_it += val;
	*v_it += val;
	EXPECT_EQ(*mv_it, *v_it);
	
	int idx = 3; //4
	mv_it[idx] += val;
	v_it[idx] += val;
	EXPECT_EQ(mv_it[idx], v_it[idx]);

	idx = 0; 
	mv_it[idx] = *mv_it;
	v_it[idx] = *v_it;
	EXPECT_EQ(mv_it[idx], *mv_it);
	EXPECT_EQ(mv_it[idx], v_it[idx]);

	idx = 2; 
	mv_it[idx] = *mv_it; // 1 = 3
	v_it[idx] = *v_it;
	EXPECT_EQ(mv_it[idx], *mv_it);
	EXPECT_EQ(mv_it[idx], v_it[idx]);

	idx = -4;
	mv_end[idx] = *mv_it; // 1 = 4
	v_end[idx] = *v_it;	
	EXPECT_EQ(mv_end[idx], *mv_it);
	EXPECT_EQ(mv_end[idx], v_end[idx]);

	idx = -1;
	mv_end[idx] = *mv_it; // 1 = 0
	v_end[idx] = *v_it;	
	EXPECT_EQ(mv_end[idx], *mv_it);
	EXPECT_EQ(mv_end[idx], v_end[idx]);


}

template <typename Vec>
void TestReserve(Vec t)
{
	Vec check(t);
	size_t prev_cap = t.capacity();	
	size_t prev_size = t.size();
	decltype(auto) prev_data = t.data();


	size_t cap = 0;
	// myvec::AllocCounter alloc_counter; 
	t.reserve(cap);
	// EXPECT_EQ(alloc_counter.allocations(), 0);
	EXPECT_EQ(t.data(), prev_data);
	EXPECT_EQ(t.capacity(), prev_cap);
	EXPECT_EQ(t.size(), prev_size);
	EXPECT_TRUE(std::equal(t.begin(), t.end(), check.begin()));

	cap = prev_cap ? prev_cap - 1 : 0;
	// alloc_counter.reset(); 
	t.reserve(cap);
	// EXPECT_EQ(alloc_counter.allocations(), 0);
	EXPECT_EQ(t.data(), prev_data);
	EXPECT_EQ(t.capacity(), prev_cap);
	EXPECT_EQ(t.size(), prev_size);
	EXPECT_TRUE(std::equal(t.begin(), t.end(), check.begin()));

	cap = prev_cap;
	// alloc_counter.reset(); 
	t.reserve(cap);
	// EXPECT_EQ(alloc_counter.allocations(), 0);
	EXPECT_EQ(t.data(), prev_data);
	EXPECT_EQ(t.capacity(), prev_cap);
	EXPECT_EQ(t.size(), prev_size);
	EXPECT_TRUE(std::equal(t.begin(), t.end(), check.begin()));
	
	cap = prev_cap;
	cap += 42;
	// alloc_counter.reset(); 
	t.reserve(cap);
	// EXPECT_EQ(alloc_counter.allocations(), 1);
	EXPECT_GE(t.capacity(), cap);
	EXPECT_EQ(t.size(), prev_size);
	EXPECT_TRUE(std::equal(t.begin(), t.end(), check.begin()));
}

template <typename T>
myvec::MyVector<T> make_my_vector_size(size_t size, int start_val)
{
	myvec::MyVector<int> vi(size);
	std::iota(vi.begin(), vi.end(), start_val);

	if constexpr (std::is_same_v<T, int>)
		return vi;
	else if constexpr (std::is_same_v<T, std::string>)
	{
		myvec::MyVector<std::string> vs(size);
		std::transform(vi.begin(), vi.end(), vs.begin(), [](auto&& vi_el){ return std::to_string(vi_el); } );
		return vs;
	}
	else
	{
		myvec::MyVector<T> vt(size);
		std::transform(vi.begin(), vi.end(), vt.begin(), [](auto&& vi_el){ return T(vi_el); } );
		return vt;
	}
}

template <typename T>
std::vector<T> make_std_vector_size(size_t size, int start_val)
{
	std::vector<int> vi(size);
	std::iota(vi.begin(), vi.end(), start_val);

	if constexpr (std::is_same_v<T, int>)
		return vi;
	else if constexpr (std::is_same_v<T, std::string>)
	{
		std::vector<std::string> vs(size);
		std::transform(vi.begin(), vi.end(), vs.begin(), [](auto&& vi_el){ return std::to_string(vi_el); } );
		return vs;
	}
	else
	{
		std::vector<T> vt(size);
		std::transform(vi.begin(), vi.end(), vt.begin(), [](auto&& vi_el){ return T(vi_el); } );
		return vt;
	}

}



template <typename Vec>
void TestPushBack(	Vec t,
					int val_start, 
					const typename Vec::value_type& val)
{

	Vec check(t);
	size_t prev_cap = t.capacity();
	size_t prev_size = t.size();
	decltype(auto) prev_data = t.data();
	
	// myvec::AllocCounter alloc_counter;
	if (prev_size < prev_cap)
	{
		// alloc_counter.reset(); 
		t.push_back(val);
		// EXPECT_EQ(alloc_counter.allocations(), 0);
		EXPECT_EQ(t.data(), prev_data);
		EXPECT_GE(t.capacity(), prev_cap);
		EXPECT_EQ(t.size(), prev_size + 1);
		EXPECT_TRUE(std::equal(t.begin(), t.end() - 1, check.begin()));
		EXPECT_EQ(t.back(), val);
		
		size_t gap = prev_cap - prev_size;
		auto vgap = make_std_vector_size<typename Vec::value_type>(gap, val_start);
		val_start += gap;

		// alloc_counter.reset();
		for (size_t i = 0; i < gap; ++i)
			t.push_back(vgap[i]);  // vgap[i] provides unique int value for Ctor of vector's element
		// EXPECT_EQ(alloc_counter.allocations(), 0);
				
		EXPECT_EQ(t.data(), prev_data);
		EXPECT_GE(t.capacity(), prev_cap);
		EXPECT_EQ(t.size(), prev_size + 1);
		EXPECT_TRUE(std::equal(t.begin(), t.end() - 1 - gap, check.begin()));
		EXPECT_TRUE(std::equal(t.end() - 1 - gap, t.end() - 1 - gap, vgap.begin()));
		
		check = t;
		prev_size = t.size();
	}
	
	ASSERT_EQ(t.size(), t.capacity());
	ASSERT_EQ(t.size(), prev_cap);

	// alloc_counter.reset();
	t.push_back(val);
	// if constexpr (std::is_trivially_copyable_v<typename Vec::value_type>)
	// 	EXPECT_EQ(alloc_counter.allocations(), 1);
	// else
	// 	EXPECT_GE(alloc_counter.allocations(), 1);

	EXPECT_GE(t.capacity(), prev_cap + 1);
	EXPECT_EQ(t.size(), prev_size + 1);
	EXPECT_TRUE(std::equal(t.begin(), t.end() - 1, check.begin()));
	EXPECT_EQ(t.back(), val);
}
// std::for_each(t.end() - 1 - gap, t.end(), [](auto&& el){ EXPECT_EQ(el, val); } );
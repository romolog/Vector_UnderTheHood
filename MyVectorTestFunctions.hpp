#include "MyVector.hpp"

#include <iterator>
#include <concepts>
#include <random>
#include <string>
#include <type_traits>
#include <vector>


// GTEST_LOG_(INFO) << "\n\tCall default ctor, check if size = 0, empty = true\n";
// EXPECT_NE(t.data(), nullptr); // ASSERT_NE(ptr, nullptr) = stop if fail


// asm volatile("" : "+m"(obj) : : "memory");
	// ^^        ^  ^^^^^^^^   	^   ^^^^^^^
	// |         |     |        |      |
	// |         |     |        |   Clobber list
	// |         |     |      Output list (empty)
	// |         |   Input/Output operand
	// |       Output operands
	// Assembly template (empty)
// 1. asm volatile
//     asm - Inline assembly statement
//     volatile - Prevents the compiler from moving or removing this asm block
//         Even if it seems useless, it must stay exactly where placed
// 2. "" - Empty assembly template
//     No actual assembly instructions are generated
//     The effect comes entirely from the constraints, not from executed code
// 3. "+m"(obj) - Input/Output operand
//     + - Means this is both read and written (input/output)
//     m - Memory operand (not a register)
//     (obj) - The variable to treat as memory operand
// 	 This tells the compiler:
//     obj is used as an input
//     obj is modified as an output
//     The modification happens through memory, not registers
// 4. Empty output list :
//     No explicit output operands beyond those in the input/output list
// 5. "memory" - Memory clobber
//     Tells the compiler: "This asm block may read/write any memory address"
//     Forces the compiler to reload all memory values from actual memory
//     Prevents keeping values in registers across this barrier
//
// OPTIONS:
// // 1. Minimum barrier (only memory clobber)
// asm volatile("" ::: "memory");
// // Stops reordering, but may not preserve individual variables

// // 2. Variable-specific barrier (your version)
// asm volatile("" : "+m"(obj) : : "memory");
// // Best for forcing operations on specific variable

// // 3. Multiple variables
// asm volatile("" : "+m"(obj1), "+m"(obj2) : : "memory");

// // 4. With input only (read barrier)
// asm volatile("" : : "m"(obj) : "memory");
// // Says: "I read obj", but don't modify it

// // 5. Maximum barrier (force all memory)
//	//	IMPORTANT: is NOT a superset of the specific operand barrier
// asm volatile("" : : : "memory");
// // All memory operations are observable
template <typename T>
__attribute__((always_inline)) 
void compiler_barrier(T& obj) {
	asm volatile("" : "+m"(obj) : : "memory");
}

template <typename T>
__attribute__((always_inline)) 
void compiler_barrier(T& obj_1, T& obj_2) {
	asm volatile("" : "+m"(obj_1), "+m"(obj_1) : : "memory");
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
};

template<typename Vec>
void TestSizeValVector(Vec& t)
{
	TestSizeVector(t, 0);
};

template<typename Vec>
void TestSizeValVector(Vec& t, size_t size, const typename Vec::value_type& val)
{

	TestSizeVector(t, size);

	std::for_each(t.begin(), t.end(), [val](auto&& el){EXPECT_EQ(el, val);});
	// EXPECT_EQ(t[0], val);

};

template<typename Vec, typename Iter>
	requires std::input_iterator<Iter>
void TestSizeValVector(Vec& t, Iter it, Iter end)
{

	TestSizeVector(t, std::distance(it, end));

	std::for_each(t.begin(), t.end(), [&it, &end](auto&& el){EXPECT_EQ(el, *it++);});

};


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

// #include <vector>
// #include <atomic>
// #include <cstring>

// class IterType {
// public:
//     int* ptr;
//     int dummy;
    
//     IterType() : ptr(nullptr), dummy(0) {}
//     IterType(int* p) : ptr(p), dummy(1) {}
    
//     IterType& operator=(const IterType& other) {
//         ptr = other.ptr;
//         dummy = other.dummy;
//         // Force observable side effect
//         asm volatile("" : : "r"(ptr), "r"(dummy) : "memory");
//         return *this;
//     }
// };

// Compiler barrier helper
// template<typename T>
// __attribute__((always_inline)) 
// void compiler_barrier(T& obj) {
//     asm volatile("" : "+m"(obj) : : "memory");
// }

// int main() {
//     std::vector<int> vector = {1, 2, 3};
    
//     iter_type it_copy_assign;
//     iter_type end_copy_assign;
    
//     // Force copy assignment to be observable
//     compiler_barrier(it_copy_assign);
//     compiler_barrier(end_copy_assign);
    
//     it_copy_assign = vector.begin();
//     end_copy_assign = vector.end();
    
//     compiler_barrier(it_copy_assign);
//     compiler_barrier(end_copy_assign);
    
//     // Use the values to prevent dead code elimination
//     asm volatile("" : : "r"(it_copy_assign.ptr), "r"(end_copy_assign.ptr));
    
//     return 0;
// }

/////////////////////////////////////////////////////////////////////////////////////////////////

// template<class Vec>
// void TestEmptyVector(Vec& t)
// {
// 	EXPECT_EQ(t.size(), 0);
// 	EXPECT_EQ(t.capacity(), 0);
// 	EXPECT_TRUE(t.empty());
// 	EXPECT_EQ(t.begin(), t.end());
// 	EXPECT_EQ(t.cbegin(), t.cend());
// 	EXPECT_EQ(t.rbegin(), t.rend());
// 	EXPECT_EQ(t.crbegin(), t.crend());

// 	int count = 0;
// 	for (auto&& _ : t)
// 	{
// 		(void)_;
// 		++count;
// 	}
// 	EXPECT_EQ(count, 0);
// };


// template <typename T>
// std::vector<T> std_vector42_shuffled(void)
// {
// 	std::random_device rd;
// 	std::mt19937 gen(rd());
// 	std::vector<int> v(42);
// 	std::iota(v.begin(), v.end(), 0);
// 	std::shuffle(v.begin(), v.end(), gen);
	
// 	if (std::is_same_v<T, int>)
// 		return v;
// 	else if (std::is_same_v<T, std::__cxx11::basic_string<char> > )
// 	{	
// 		auto iter = v.begin();
// 		std::uniform_int_distribution<int> i_dist(0, 25);				
		
// 		auto make_random_string = [&iter, &i_dist, &gen]()
// 		{
// 			size_t len = *iter;
// 			++iter; 
// 			std::string str;
// 			str.reserve(len);

// 			char rand_ch = i_dist(gen);
// 			str.push_back('A' + rand_ch);
			
// 			auto make_random_lowercase = [&i_dist, &gen]()
// 			{
// 				char rand_ch = i_dist(gen);
// 				return ('a' + rand_ch);
// 			};

// 			std::generate(str.begin() + 1, str.end(), make_random_lowercase);

// 			return str;
// 		};

// 		std::vector<T> vt(42);
// 		std::generate(vt.begin(), vt.end(), make_random_string);

// 		return vt;
// 	}
// 	else
// 	{
// 		std::vector<T> vt(42);
// 		auto iter = v.begin();
// 		std::generate(vt.begin(),vt.end(),[&iter](){ T(*iter++); });
// 		return vt;
// 	}
// }


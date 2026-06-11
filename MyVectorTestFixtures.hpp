#pragma once

#include "MyVector.hpp"
#include "MyVectorTestClasses.hpp"

#include <array>
#include <concepts>
#include <initializer_list>
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <type_traits>
#include <vector>
#include <utility>




template<typename T>
class TT_1 : public ::testing::Test
{
	protected:
		constexpr T value(int n) 
		{
			if constexpr (std::is_same_v<T, std::string>)
			{
				return n ?
					  std::to_string(n)
					: std::string();
			}
			else
				return T(n);
		}

		constexpr std::vector<T> std_vector_size42()
		{
			std::vector<int> v(42);
			std::iota(v.begin(), v.end(), 0);
			
			if constexpr (std::is_same_v<T, int>)
				return v;
			else 
			{
				std::vector<T> vt(42);
				auto it = v.begin();
				if constexpr (std::is_same_v<T, std::string>)
					std::generate(vt.begin(), vt.end(), [&it]() { return std::to_string(*it++); } );
				else
					std::generate(vt.begin(), vt.end(), [&it]() { return T(*it++); } );
				return vt;
			}
		}

		constexpr static std::initializer_list<int> il_3int = {37, 42, 73};
		constexpr static std::initializer_list<int> il_7int = {1, 2, 3, 4, 5, 6, 7};

		constexpr static std::initializer_list<std::string> il_3str = {"a", "ab", "abc"};
		constexpr static std::initializer_list<std::string> il_7str = {"Hello", "World", "!", "hello_world", "Hello World !", "Hi", "Good day"};

		template <typename U>
		constexpr static std::initializer_list<U> il_3T = { U(1), U(2), U(3)};

		template <typename U>
		constexpr static std::initializer_list<U> il_7T = { U(1), U(2), U(3), U(4), U(5), U(6), U(7)};

		constexpr std::initializer_list<T> std_init_list_3()
		{
			if constexpr (std::is_same_v<T, int>)
				return il_3int;
			else if constexpr (std::is_same_v<T, std::string>)
				return il_3str;
			else
				return il_3T<T>;
		}

		constexpr std::initializer_list<T> std_init_list_7()
		{
			if constexpr (std::is_same_v<T, int>)
				return il_7int;
			else if constexpr (std::is_same_v<T, std::string>)
				return il_7str;
			else
				return il_7T<T>;
		}


		// template<int ... Args>
		// constexpr const std::initializer_list<std::string>& seq_to_stringlist (std::integer_sequence<int, Args ...>) 
		// {
		// 	return std::initializer_list<std::string>{ std::to_string(Args + 1) ... };
		// }


		// template<typename U, int ... Args>
		// constexpr const std::initializer_list<U>& seq_to_Tlist (std::integer_sequence<int, Args ...>) 
		// {
		// 	return std::initializer_list<U>{ U(Args + 1) ... };
		// }



		// constexpr const std::initializer_list<T>& std_init_list42()
		// {

		// 	// template<int ... Args>
		// 	// auto seq_to_intlist = 	[](std::integer_sequence<int, Args ...>) 
		// 	// 						{
		// 	// 							return std::initializer_list<int>{ (Args + 1) ... };
		// 	// 						};

		// 	// if constexpr (std::is_same_v<T, int>) {
		// 	// 	return seq_to_intlist(std::make_integer_sequence<int, 42>{});
		// 	// }


		// 	if constexpr (std::is_same_v<T, int>) 
		// 	{
		// 		return	[]<size_t ... Args>(std::index_sequence<Args ... >)
		// 				-> const std::initializer_list<int>&
		// 				{
		// 					return std::initializer_list<int>{ (Args + 1) ... };
		// 				}(std::make_index_sequence<42>{});
		// 	}
		// 	else if constexpr (std::is_same_v<T, std::string>)
		// 		return seq_to_stringlist(std::make_integer_sequence<int, 42>{});
		// 	else
		// 		return seq_to_Tlist<T>(std::make_integer_sequence<int, 42>{});
		// 	// else
		// 	// 	return seq_to_stringlist(std::make_integer_sequence<int, 42>{});
			


		// 	// std::array<int, 42> arr;
		// 	// std::iota(arr.begin(), arr.end(), 0);

		// 	// if constexpr (std::is_same_v<T, int>)
		// 	// 	return std::initializer_list<int>(arr.data(), 42);
		// 	// else 
		// 	// {
		// 	// 	std::array<T, 42> arr;
		// 	// 	auto it = arr.begin();
		// 	// 	if constexpr (std::is_same_v<T, std::string>)
		// 	// 		std::generate(arr.begin(), arr.end(), [&it]() { return std::to_string(*it++); } );
		// 	// 	else
		// 	// 		std::generate(arr.begin(), arr.end(), [&it]() { return T(*it++); } );
		// 	// 	return std::initializer_list<T>(arr.data(), 42);;
		// 	// }
		// }



};

// class VectorTest : public ::testing::Test
// {
// 	protected:
// 		myvec::MyVector<int> v;

// 		void SetUp() override
// 		{
// 			// runs before each test
// 		}

// 		void TearDown() override
// 		{
// 			// runs after each test
// 		}
// };


				// for (int k = 0; k < len; ++k)
				// {
				// 	std::string str;
				// 	str.reserve(v[k]);
				// 	char rand_ch = i_dist(gen);
				// 	str.push_back('A' + rand_ch);					
				// 	std::fill(str.begin() + 1, str.end(), make_random_lowercase);
				// 	// for (int i = 1; i < len; ++i)
				// 	// {
				// 	// 	char rand_ch = i_dist(gen);
				// 	// 	str.push_back('a' + rand_ch);
				// 	// }
				// 	vt.push_back(str);
				// }


		// 				T value_random_nonzero(void) 
		// {
		// 	std::random_device rd;
		// 	std::mt19937 gen(rd()); // gen(73) if deterministic needed
		// 	std::uniform_int_distribution<int> len_dist(1, 43);
		// 	int rand_len = len_dist(gen);
			
		// 	// std::vector<T> v(42);
		// 	// std::iota(v.begin(), v.end(), 37);
		// 	// std::shuffle(v.begin(), v.end(), gen);

		// 	if constexpr (std::is_same_v<T, std::string>)
		// 	{	
		// 		std::string str;
		// 		str.reserve(rand_len);
		// 		std::uniform_int_distribution<int> i_dist(0, 25);

		// 		char rand_ch = i_dist(gen);
		// 		str.push_back('A' + rand_ch);

		// 		for (int i = 1; i < rand_len; ++i)
		// 		{
		// 			char rand_ch = i_dist(gen);
		// 			str.push_back('a' + rand_ch);
		// 		}
		// 		return str;
		// 	}
		// 	else
		// 		return T(rand_len);
		// }

		// // shuffled 
		// std::vector<T> std_vector42_shuffled(void)
		// {
		// 	std::random_device rd;
		// 	std::mt19937 gen(rd());
		// 	std::vector<int> v(42);
		// 	std::iota(v.begin(), v.end(), 0);
		// 	std::shuffle(v.begin(), v.end(), gen);
			
		// 	if constexpr (std::is_same_v<T, int>)
		// 		return v;
		// 	else if (std::is_same_v<T, std::__cxx11::basic_string<char> >)
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
		// 		std::generate(vt.begin(),vt.end(), make_random_string);


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
#pragma once

#include "MyVector.hpp"

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
};

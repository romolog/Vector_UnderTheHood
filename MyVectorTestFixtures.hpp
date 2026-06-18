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
class Regular_ : public ::testing::Test
{
	protected:
		constexpr T value(const int n) 
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

		myvec::MyVector<T> make_vector_size(size_t size, int start_val)
		{

			myvec::MyVector<int> v(size);
			std::iota(v.begin(), v.end(), start_val);
			
			if constexpr (std::is_same_v<T, int>)
				return v;
			else if constexpr (std::is_same_v<T, std::string>)
			{
				myvec::MyVector<std::string> vs;
				auto it = v.begin();
				std::generate(vs.begin(), vs.end(), [&it]() { return std::to_string(*it++); } );
				return vs;			
			}
			else
			{
				myvec::MyVector<T> vt(size);
				auto it = v.begin();
				std::generate(vt.begin(), vt.end(), [&it]() { return T(*it++); } );
				return vt;
			}
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
}; 
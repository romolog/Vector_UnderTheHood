#include "MyVector.hpp"

#include "MyVectorTestAllocators.hpp"
#include "MyVectorTestClasses.hpp"

#include <iostream>
#include <vector>

int main(void)
{
	{
		myvec::MyVector<std::string> mv_str_3{"Hello", "World", "!"};
		myvec::MyVector<std::string> mv_str_5{"C", "C++", "RAII", "AAA", "SFINAE"};
		mv_str_3 = std::move(mv_str_5);
	}

	{
		myvec::AllocThrow<int> throwing_alloc;
		myvec::AllocThrow<int>::allocs_before_throw_ = 1;
		myvec::MyVector<int, myvec::AllocThrow<int> > mv_int(37, 73, throwing_alloc);
		try
		{
			mv_int.reserve(mv_int.capacity() + 42);
		}
		catch(const std::bad_alloc& e)
		{
			std::cout	<< "\n\t Reserve(): \n\t\t"
						<< "Exception caught: "<< e.what() << "\n\n";
		}
	}
	
	{
		myvec::AllocThrow<int> throwing_alloc;
		myvec::AllocThrow<int>::allocs_before_throw_ = 1;
		myvec::MyVector<int, myvec::AllocThrow<int> > mv_int(37, 73, throwing_alloc);
		try
		{
			mv_int.push_back(42);
		}
		catch(const std::bad_alloc& e)
		{
			std::cout	<< "\n\t Push_back(): \n\t\t"
						<< "Exception caught: "<< e.what() << "\n\n";
		}
	}

	{
		int n = 7;
		using T = myvec::StructThrow;
		T::copies_moves_before_throw_ = n;
		myvec::MyVector<T > mv_throw(n - 2, T());
		try
		{
			myvec::MyVector<T > mv_throw_copy(mv_throw);
		}
		catch(const T::Exception& e)
		{
			std::cout	<< "\n\t Copy: \n\t\t"
						<< "Exception caught: "<< e.what() << "\n\n";
		}
	}

	{
		int n = 7;
		using T = myvec::StructThrow;
		T::copies_moves_before_throw_ = n;
		myvec::MyVector<T > mv_throw(n - 1, T());
		try
		{
			mv_throw.reserve(mv_throw.capacity() + 42);
		}
		catch(const T::Exception& e)
		{
			std::cout	<< "\n\t Reserve: \n\t\t"
						<< "Exception caught: "<< e.what() << "\n\n";
		}
	}

	{
		int n = 7;
		using T = myvec::StructThrow;
		T::copies_moves_before_throw_ = n;
		myvec::MyVector<T > mv_throw(n - 1, T());
		try
		{
			mv_throw.push_back(T());
		}
		catch(const T::Exception& e)
		{
			std::cout	<< "\n\t Push_back: \n\t\t"
						<< "Exception caught: "<< e.what() << "\n\n";
		}
	}
	return 0;
}


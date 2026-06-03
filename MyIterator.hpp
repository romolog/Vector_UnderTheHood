#pragma once
#include "MyVector.hpp"
#include <iostream>

namespace myvec
{
	template <typename T, typename Allocator>
	class MyVector;

	template <typename T, typename Pointer = T*, typename Reference = T&>
	class MyIterator final
	{
		private:
			T* dataPtr_ = nullptr;

		public:
			using value_type 		= T;
			using difference_type	= std::ptrdiff_t;
			using pointer 			= Pointer;
			using reference 		= Reference;
			using iterator_category = std::random_access_iterator_tag;
			using iterator_concept 	= std::contiguous_iterator_tag;

			MyIterator() noexcept = default;
			MyIterator(T* ptr) noexcept : dataPtr_(ptr) {}

			MyIterator(MyIterator&& move) = default;
			MyIterator& operator=(MyIterator&& move) = default;	

			MyIterator(const MyIterator& copy) = default;
			MyIterator& operator=(const MyIterator& copy) = default;

			reference 			operator*	() const noexcept { return *dataPtr_;}
			reference			operator[]	(difference_type idx) const noexcept { return *(dataPtr_ + idx);}

			MyIterator&			operator++	() noexcept { ++dataPtr_; return *this; }

			MyIterator			operator++	(int) noexcept 
			{
				MyIterator prev(*this);
				++dataPtr_;
				return prev;
			}

			MyIterator&			operator--	() noexcept { --dataPtr_; return *this; }

			//	just for fun, fragile 
			//	for multiple fileds better make explicity snapshot the state
			//		MyClass prev(*this);
			MyIterator			operator--	(int) noexcept { return MyIterator(dataPtr_--); }

			auto operator<=>(const MyIterator& second) const noexcept = default; 
			// automatically: == != > >= < <=

			MyIterator&			operator+=	(const difference_type shift) noexcept
			{	
				dataPtr_ += shift;
				return *this;
			}

			// +n = copy + advance(n)
			// operator+ : implemented by operator+=
			MyIterator			operator+	(const difference_type shift) noexcept
			{	
				MyIterator res(*this);
				res += shift;
				return res;
			}

			// IMPORTANT: support num + iter: 
			friend 
			MyIterator	operator+(difference_type shift, MyIterator it) noexcept { return (it += shift);}

			MyIterator&			operator-=	(const difference_type shift) noexcept 
			{	
				dataPtr_ -= shift;
				return *this;
			}

			MyIterator			operator-	(const difference_type shift) noexcept
			{	
				MyIterator res(*this);
				res -= shift;
				return res;
			}
			
			difference_type 	operator-	(const MyIterator& rhs) noexcept { return (dataPtr_ - rhs.dataPtr_); }

	};

	// template <typename T>
	// class MyConstIterator final
	// {
	// 	private:
	// 		const T* dataPtr_;

	// 	public:
	// 		//IMPORTANT:
	// 		//	value_type = what is stored in container, not dereference type of iterator
	// 		using value_type 		= T;
	// 		using difference_type	= std::ptrdiff_t;
	// 		using pointer 			= const T*;
	// 		using reference 		= const T&;
	// 		using iterator_category = std::random_access_iterator_tag;
	// 		using iterator_concept 	= std::contiguous_iterator_tag;

	// 		MyConstIterator(const T* ptr) : dataPtr_(ptr) {};

	// 		MyConstIterator(MyConstIterator&& move) = default;
	// 		MyConstIterator& operator=(MyConstIterator&& move) = default;	

	// 		MyConstIterator(const MyConstIterator& copy) = default;
	// 		MyConstIterator& operator=(const MyConstIterator& copy) = default;

	// 		reference			operator*	() const noexcept { return *dataPtr_;}
	// 		reference			operator[]	(difference_type idx) const noexcept { return *(dataPtr_ + idx);}

	// 		MyConstIterator&	operator++	() noexcept { ++dataPtr_; return *this; }

	// 		MyConstIterator		operator++	(int) noexcept 
	// 		{
	// 			MyConstIterator prev(*this);
	// 			++dataPtr_;
	// 			return prev;
	// 		}

	// 		MyConstIterator&	operator--	() noexcept { --dataPtr_; return *this; }

	// 		MyConstIterator		operator--	(int) noexcept { return MyConstIterator(dataPtr_--); }

	// 		auto operator<=>(const MyConstIterator& second) const noexcept = default; 

	// 		MyConstIterator&	operator+=	(const difference_type shift) noexcept
	// 		{	
	// 			dataPtr_ += shift;
	// 			return *this;
	// 		}

	// 		MyConstIterator		operator+	(const difference_type shift) noexcept
	// 		{	
	// 			MyConstIterator res(*this);
	// 			res += shift;
	// 			return res;
	// 		}

	// 		friend 
	// 		MyConstIterator	operator+(difference_type shift, MyConstIterator it) noexcept { return (it += shift);}

	// 		MyConstIterator&	operator-=	(const difference_type shift) noexcept 
	// 		{	
	// 			dataPtr_ -= shift;
	// 			return *this;
	// 		}

	// 		MyConstIterator		operator-	(const difference_type shift) noexcept
	// 		{	
	// 			MyConstIterator res(*this);
	// 			res -= shift;
	// 			return res;
	// 		}
			
	// 		difference_type 	operator-	(const MyConstIterator& rhs) noexcept { return (dataPtr_ - rhs.dataPtr_); }

	// };


template<class T>
concept Decrementable = requires(T t) {
    --t;
};

	template<class Iterator>
	requires requires(Iterator t) {--t;}
	class MyReverseIterator final
	{
		private:
			Iterator current_;

		public:
			using value_type 		= Iterator::value_type;
			using difference_type	= Iterator::difference_type;
			using pointer 			= Iterator::pointer;
			using reference 		= Iterator::reference;
			using iterator_category = Iterator::iterator_category;
			using iterator_concept 	= Iterator::iterator_concept;


			MyReverseIterator() = default;

			MyReverseIterator(Iterator& it): current_(it) {}

			Iterator base() const { return current_;}


			// int x = 10;
			// int& r = x;
			// auto a = r;            // int
			// decltype(auto) b = r;  // int&
			// decltype(auto) operator*() const

			reference operator*() const
			{
				MyIterator tmp = current_;
				--tmp;
				return *tmp;
			}
	};

	// template<class MyIterator>
	// class reverse_iterator {
	// private:
	// 	Iterator current;

	// public:
	// 	reverse_iterator() = default;

	// 	explicit reverse_iterator(Iterator it)
	// 		: current(it)
	// 	{}

	// 	Iterator base() const {
	// 		return current;
	// 	}

	// 	decltype(auto) operator*() const
	// 	{
	// 		Iterator tmp = current;
	// 		--tmp;
	// 		return *tmp;
	// 	}
	// };
}
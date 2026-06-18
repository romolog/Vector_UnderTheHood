#pragma once
#include "MyVector.hpp"

#include <concepts>
#include <iostream>
#include <iterator>
#include <type_traits>


namespace myvec
{
	template <typename T, typename Allocator>
	class MyVector;

	template <typename T, typename Pointer = T*, typename Reference = T&>
	class MyIterator final
	{
		protected:
			Pointer dataPtr_ = nullptr;

		public:
			using value_type 		= T;
			using difference_type	= std::ptrdiff_t;
			using pointer 			= Pointer;
			using reference 		= Reference;
			using iterator_category = std::random_access_iterator_tag;
			using iterator_concept 	= std::contiguous_iterator_tag;

			MyIterator() noexcept = default;
			MyIterator(Pointer ptr) noexcept : dataPtr_(ptr) {}

			MyIterator(MyIterator&& move) = default;
			MyIterator& operator=(MyIterator&& move) = default;	

			MyIterator(const MyIterator& copy) = default;
			MyIterator& operator=(const MyIterator& copy) = default;

			auto operator<=>(const MyIterator& second) const noexcept = default; 

			reference 			operator*	() const noexcept { return *dataPtr_;}

			reference			operator[]	(difference_type idx) const noexcept { return *(dataPtr_ + idx);}

			MyIterator&			operator++	() noexcept { ++dataPtr_; return *this; }

			MyIterator			operator++	(int) noexcept 
			{
				MyIterator prev(*this);
				operator++();
				return prev;
			}

			MyIterator&			operator--	() noexcept { --dataPtr_; return *this; }

			MyIterator			operator--	(int) noexcept { return MyIterator(dataPtr_--); }

			MyIterator&			operator+=	(const difference_type shift) noexcept
			{	
				dataPtr_ += shift;
				return *this;
			}

			MyIterator			operator+	(const difference_type shift) const noexcept
			{	
				MyIterator res(*this);
				res += shift;
				return res;
			}

			friend 
			MyIterator	operator+(difference_type shift, MyIterator it) noexcept { return (it += shift);}

			MyIterator&			operator-=	(const difference_type shift) noexcept 
			{ *this += -shift; return *this; }

			MyIterator			operator-	(const difference_type shift) const noexcept
			{	
				MyIterator res(*this);
				res -= shift;
				return res;
			}
			
			difference_type 	operator-	(const MyIterator& rhs) const noexcept { return (dataPtr_ - rhs.dataPtr_); }
	}; // MyIterator ends
} // namespace myvec ends


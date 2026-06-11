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
		// friend MyVector;

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

			// int x = 10;
			// int& r = x;
			// auto a = r;            // int
			// decltype(auto) b = r;  // int&
			// decltype(auto) operator*() const
			reference 			operator*	() const noexcept { return *dataPtr_;}

			reference			operator[]	(difference_type idx) const noexcept { return *(dataPtr_ + idx);}

			// TODO : operator->

			MyIterator&			operator++	() noexcept { ++dataPtr_; return *this; }

			MyIterator			operator++	(int) noexcept 
			{
				MyIterator prev(*this);
				operator++();
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
			MyIterator			operator+	(const difference_type shift) const noexcept
			{	
				MyIterator res(*this);
				res += shift;
				return res;
			}

			// IMPORTANT: support num + iter: 
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

	};


	// old technique
	//		could be replaced: std::bidirectional_iterator<Iter>;
	template <class Iter>
	concept Reversible = 	std::is_base_of_v<	std::bidirectional_iterator_tag, 
												typename std::iterator_traits<Iter>::iterator_category>;

	template <class Iter>
	concept Bidirect = 	std::bidirectional_iterator<Iter> 
					&& !std::random_access_iterator<Iter>;

	template <class Iter>
	concept RandomAccess = std::random_access_iterator<Iter>;


	template <class Iterator>
		requires Reversible<Iterator>
	class MyReverseIterator
	{
		protected:
			Iterator current_;

		public:
			using value_type 		= Iterator::value_type;
			using difference_type	= Iterator::difference_type;
			using pointer 			= Iterator::pointer;
			using reference 		= Iterator::reference;
			using iterator_category = Iterator::iterator_category;
			using iterator_concept 	= Iterator::iterator_concept;

			MyReverseIterator() noexcept = default;
			MyReverseIterator(Iterator&& it) noexcept : current_(it) {}
	
			MyReverseIterator(MyReverseIterator&& move) = default;
			MyReverseIterator& operator=(MyReverseIterator&& move) = default;	

			MyReverseIterator(const MyReverseIterator& copy) = default;
			MyReverseIterator& operator=(const MyReverseIterator& copy) = default;

			Iterator base() const { return current_;}


			reference		operator*	() 
				const 
				noexcept
				requires RandomAccess<Iterator>  
			{ return *(current_ - 1); }

			reference 		operator*	() 
				const
				noexcept
				requires Bidirect<Iterator>
			{
				Iterator tmp{current_};
				--tmp;
				return *tmp;
			}


			reference		operator[]	(difference_type idx) 
				const 
				noexcept 
				requires RandomAccess<Iterator>  
			{ return *(current_ - 1 - idx); }

			reference		operator[]	(difference_type idx) 
				const 
				noexcept 
				requires Bidirect<Iterator>
			{ 
				Iterator tmp{current_};
				if (idx > 0)
					for (difference_type i = 0; i < idx; ++i)
						--tmp;
				else
					for (difference_type i = 0; i > idx; --i)
						++tmp;
				return *tmp;
			}


			MyReverseIterator&	operator++	() noexcept { --current_; return *this; }

			MyReverseIterator	operator++	(int) noexcept 
			{
				MyReverseIterator prev(*this);
				--current_;
				return prev;
			}

			MyReverseIterator&	operator--	() noexcept { ++current_; return *this; }

			MyReverseIterator	operator--	(int) noexcept 
			{ 
				MyReverseIterator prev(*this);
				++current_;
				return prev; 
			}

			auto operator<=>(const MyReverseIterator& second) const noexcept = default; 

			// just for fun
			// bool operator==(const MyReversIterator& rhs)
			// {
			//		return dataPtr == rhs.dataPtr;
			// }

			MyReverseIterator&	operator+=	(const difference_type shift) 
				noexcept 
				requires RandomAccess<Iterator>
			{ current_ -= shift; return *this; }

			MyReverseIterator&	operator+=	(const difference_type shift) 
				noexcept 
				requires Bidirect<Iterator>
			{ 
			
				Iterator tmp{current_};
				if (shift > 0)
					for (difference_type i = 0; i < shift; ++i)
						--tmp;
				else
					for (difference_type i = 0; i > shift; --i)
						++tmp; 
				return *this; 
			}

			MyReverseIterator	operator+	(const difference_type shift) const noexcept
			{	
				MyReverseIterator res(*this);
				res += shift;
				return res;
			}

			friend 
			MyReverseIterator	operator+(difference_type shift, MyReverseIterator rit) noexcept 
			{ return (rit += shift);}

			MyReverseIterator&	operator-=	(const difference_type shift) noexcept 
			{ (*this) += -shift; return *this; }

			MyReverseIterator	operator-	(const difference_type shift) const noexcept
			{	
				MyReverseIterator res(*this);
				res -= shift;
				return res;
			}
			
			difference_type 	operator-	(const MyReverseIterator& rhs) const noexcept 
			{ return (rhs.current_ - current_); }
	};

	


} // namespace myvec



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


	// template<>
	// class MyReverseIterator<Reversible Iterator>
	// {
	// 	private:
	// 		Iterator current_;

	// 	public:
	// 		using value_type 		= Iterator::value_type;
	// 		using difference_type	= Iterator::difference_type;
	// 		using pointer 			= Iterator::pointer;
	// 		using reference 		= Iterator::reference;
	// 		using iterator_category = Iterator::iterator_category;
	// 		using iterator_concept 	= Iterator::iterator_concept;

	// 		MyReverseIterator() noexcept = default;
	// 		MyReverseIterator(const Iterator& it) noexcept : current_(it) {}

	// 		MyReverseIterator(MyReverseIterator&& move) = default;
	// 		MyReverseIterator& operator=(MyReverseIterator&& move) = default;	

	// 		MyReverseIterator(const MyReverseIterator& copy) = default;
	// 		MyReverseIterator& operator=(const MyReverseIterator& copy) = default;

	// 		Iterator base() const { return current_;}

	// 		reference 			operator*	() const noexcept 
	// 		{ 
	// 			//	for MyVector
	// 			//		no template, just use MyVectorIterator current_;
	// 			//		then *reverse_iterator: return *(current - 1);
	// 			//	but generally bidirectional iter may not support 'iter - ptrdiff_t'
	// 			Iterator tmp{current_};
	// 			--tmp;
	// 			return *tmp;
	// 		}
			
	// 		reference			operator[]	(difference_type idx) const noexcept 
	// 		{ 
	// 			Iterator tmp{current_};
	// 			if (idx > 0)
	// 				for (difference_type i = 0; i < idx; ++i)
	// 					--tmp;
	// 			else
	// 				for (difference_type i = 0; i > idx; --i)
	// 					++tmp;
	// 			return *tmp;
	// 		}

	// 		MyReverseIterator&	operator++	() noexcept { --current_; return *this; }

	// 		MyReverseIterator	operator++	(int) noexcept 
	// 		{
	// 			MyReverseIterator prev(*this);
	// 			--current_;
	// 			return prev;
	// 		}

	// 		MyReverseIterator&	operator--	() noexcept { ++current_; return *this; }

	// 		MyReverseIterator	operator--	(int) noexcept 
	// 		{ 
	// 			MyReverseIterator prev(*this);
	// 			++current_;
	// 			return prev; 
	// 		}

	// 		auto operator<=>(const MyReverseIterator& second) const noexcept = default; 

	// 		MyReverseIterator&	operator+=	(const difference_type shift) noexcept 
	// 		{	
	// 			Iterator tmp{current_};
	// 			if (shift > 0)
	// 				for (difference_type i = 0; i < shift; ++i)
	// 					--tmp;
	// 			else
	// 				for (difference_type i = 0; i > shift; --i)
	// 					++tmp; 
	// 			return *this; 
	// 		}

	// 		MyReverseIterator	operator+	(const difference_type shift) noexcept
	// 		{	
	// 			MyReverseIterator res(*this);
	// 			res += shift;
	// 			return res;
	// 		}

	// 		friend 
	// 		MyReverseIterator	operator+(difference_type shift, MyReverseIterator rit) noexcept 
	// 		{ return (rit += shift);}

	// 		MyReverseIterator&	operator-=	(const difference_type shift) noexcept 
	// 		{ *this += -shift; return *this; }

	// 		MyReverseIterator	operator-	(const difference_type shift) noexcept
	// 		{	
	// 			MyReverseIterator res(*this);
	// 			res -= shift;
	// 			return res;
	// 		}
			
	// 		difference_type 	operator-	(const MyReverseIterator& rhs) noexcept 
	// 		{ return (current_ - rhs.current_); }
	// };
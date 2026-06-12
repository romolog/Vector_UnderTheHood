#pragma once

#include "StorageRAII.hpp"
#include "MyIterator.hpp"

#include <algorithm> 
#include <concepts>
#include <cstddef> 
#include <cstring> 
#include <iterator>
#include <type_traits>
#include <utility> 

namespace myvec
{

template <typename T, typename Pointer, typename Reference>
class MyIterator;

template <typename T, typename Allocator = std::allocator<T> >
class MyVector
{
	private:
		using StorageRAII = myvec::StorageRAII<T, Allocator>;
		StorageRAII storage_;

	public:
		using value_type = T;
		using allocator = Allocator;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = StorageRAII::AllocTraits::pointer;		
		using const_pointer = StorageRAII::AllocTraits::const_pointer;

		using iterator = MyIterator<T, T*, T&>;
		using const_iterator = MyIterator<T, const T*, const T&>;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		constexpr MyVector() noexcept(noexcept(Allocator())) : MyVector(Allocator()) {}

		constexpr explicit MyVector(const Allocator& alloc ) { storage_.alloc_ = alloc; }

		explicit MyVector(size_t size, const Allocator& alloc = Allocator())
			requires std::default_initializable<T>
		{
			if (size)
			{
				storage_ = StorageRAII(size, alloc);	
				for(; storage_.size_ < size; ++storage_.size_)
					StorageRAII::AllocTraits::construct(storage_.alloc_, storage_.data_ + storage_.size_);
			}
		}

		constexpr MyVector(size_t count, const T& val, const std::type_identity_t<Allocator>& alloc = Allocator())
			requires std::copy_constructible<T>
		{
			if (count)
			{
				storage_ = StorageRAII(count, alloc);
				for(; storage_.size_ < count; ++storage_.size_)
					StorageRAII::AllocTraits::construct(storage_.alloc_, storage_.data_ + storage_.size_, val);
			}
		}

		template < typename InputIt >
		constexpr MyVector(InputIt first, InputIt last, const std::type_identity_t<Allocator>& alloc = Allocator()) 
			requires std::input_iterator<InputIt>
		{
			if (first != last)
			{
				storage_ = StorageRAII(std::distance(first, last), alloc);
				for(; first != last; ++first, ++storage_.size_)
					StorageRAII::AllocTraits::construct(storage_.alloc_, storage_.data_ + storage_.size_, *first);
			}
		}

		constexpr MyVector(MyVector&& move) noexcept : storage_(std::move(move.storage_)) {}

		constexpr MyVector(MyVector&& move, const std::type_identity_t<Allocator>& alloc) noexcept 
			: storage_(std::move(move.storage_, alloc)) 
		{}

		MyVector& operator=(MyVector&& move) 
		{
			storage_ = std::move(move.storage_);
			return *this;
		}


	private:
		class Blocking {};
	public:
		using IfCopyConstructibleConditional = 
				std::conditional_t<std::is_copy_constructible_v<T>, MyVector, Blocking>;
				
		constexpr MyVector(const IfCopyConstructibleConditional& copy) 
			: storage_(StorageRAII(copy.storage_.size_))
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				std::memcpy(storage_.data_, copy.storage_.data_, copy.storage_.size_ * sizeof(T));
				storage_.size_     = copy.storage_.size_;
			}
			else
			{
				for(; storage_.size_ < copy.storage_.size_; ++storage_.size_)
					StorageRAII::AllocTraits::construct(storage_.alloc_, 
														storage_.data_ + storage_.size_, 
														copy.storage_.data_[storage_.size_]);
			}
		}

		constexpr MyVector(	const MyVector& copy, 
							const std::type_identity_t<Allocator>& alloc) 
			requires std::copy_constructible<T> 
			: storage_(StorageRAII(copy.storage_.size_, alloc))
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				std::memcpy(storage_.data_, copy.storage_.data_, copy.storage_.size_ * sizeof(T));
				storage_.size_     = copy.storage_.size_;
			}
			else
			{
				for (; storage_.size_ < copy.storage_.size_; ++storage_.size_)
					StorageRAII::AllocTraits::construct(	storage_.alloc_, 
															storage_.data_ + storage_.size_, 
															copy.storage_.data_[storage_.size_]);
			}
		}

		MyVector& operator=(const MyVector& copy)
			requires std::copy_constructible<T>
		{

			std::cout << "\n\tMyVector Assign Operator\n\n";
			MyVector newCopy{copy};
			*this = std::move(newCopy); 
			return *this;
		}


		MyVector(	std::initializer_list<T> init_list, 
					const std::type_identity_t<Allocator>& alloc = Allocator()) 
			requires std::copy_constructible<T>
			: storage_(StorageRAII(init_list.size(), alloc))
		{
			std::for_each(	init_list.begin(), 
							init_list.end(), 
							[this](auto&& val) { 
								StorageRAII::AllocTraits::construct(storage_.alloc_, storage_.data_ + storage_.size_, val);
								++storage_.size_;
							} );
		}

		iterator 				begin() 			noexcept { return (storage_.data_); }
		iterator 				end() 				noexcept { return (storage_.data_ + storage_.size_); }

		const_iterator 			begin() 	const 	noexcept { return (storage_.data_); }
		const_iterator 			end() 		const 	noexcept { return (storage_.data_ + storage_.size_); }
		const_iterator 			cbegin() 	const 	noexcept { return (storage_.data_); }
		const_iterator 			cend()		const 	noexcept { return (storage_.data_ + storage_.size_); }

		reverse_iterator 		rbegin() 			noexcept 
											{ return std::reverse_iterator(storage_.data_ + storage_.size_); }
		reverse_iterator 		rend() 				noexcept 
											{ return std::reverse_iterator(storage_.data_); }

		const_reverse_iterator 	rbegin()	const 	noexcept 
											{ return std::reverse_iterator(storage_.data_ + storage_.size_); }
		const_reverse_iterator 	rend()		const	noexcept 
											{ return std::reverse_iterator(storage_.data_); }
		const_reverse_iterator 	crbegin()	const	noexcept 
											{ return std::reverse_iterator(storage_.data_ + storage_.size_); }
		const_reverse_iterator	crend()		const	noexcept 
											{ return std::reverse_iterator(storage_.data_); }


		T& operator[](size_t id) noexcept {return storage_.data_[id];}
		const T& operator[](size_t id) const noexcept {return storage_.data_[id];}


//------------------------------------------------------------------------------------------------
		size_t	size		(void)	const	noexcept { return storage_.size_; }

		size_t	capacity	(void)	const	noexcept { return storage_.capacity_; }

		T*		data		(void)	const	noexcept { return storage_.data_; }

//------------------------------------------------------------------------------------------------
		
		T&				back		(void) { return *(end() - 1);};
		const T&		back		(void) const { return *(end() - 1);};

		constexpr void	pop_back	(void) 
		{
			StorageRAII::AllocTraits::destroy(storage_.alloc_, storage_.data_ + storage_.size_ - 1); 
			--storage_.size_;
		}


		void	clear		(void)	noexcept 
		{ 
			for (size_t i = 0; i < storage_.size_; ++i)
				storage_.data_[i].~T();
			
			storage_.capacity_ = 0; 
			storage_.size_ = 0; 
		};

		bool	empty		(void)	const 	noexcept { return storage_.size_ == 0; }

		template <class ... Args>
		constexpr iterator emplace(const_iterator pos, Args&& ... args)
		{
			size_t idx_emplaced = std::distance(cbegin(), pos);
			size_t temp_len = storage_.size_ + 1;

			if (storage_.size_ == storage_.capacity_)
			{
				StorageRAII temp(temp_len);
	
				copy_or_move(temp, temp.size_, idx_emplaced);
				
				StorageRAII::AllocTraits::construct(	temp.alloc_, 
														temp.data_ + idx_emplaced, 
														std::forward<Args>(args) ... );
				++temp.size_;
				
				copy_or_move(temp, temp.size_, temp_len);
	
				storage_ = std::move(temp);
				
				iterator res(storage_.data_ + idx_emplaced);
				return res;
			}
			else if (pos == cend())
			{
				StorageRAII::AllocTraits::construct(	storage_.alloc_, 
														storage_.data_ + storage_.size_, 
														std::forward<Args>(args) ... );
				++storage_.size_;
			}
			else
			{
				StorageRAII temp(1);
				StorageRAII::AllocTraits::construct(	temp.alloc_, 
														temp.data_, 
														std::forward<Args>(args) ... );
				copy_or_move_storage_right(idx_emplaced);
				storage_.data_[idx_emplaced] = std::move(temp.data_[0]);
			}
		}

		private:
		void copy_or_move_storage_right (size_t idx)
		{
			size_t last = storage_.data_.size_;
			if constexpr ( 		std::is_nothrow_move_constructible_v<T> 
							&& !std::is_trivially_copyable_v<T>)
			{
				StorageRAII::AllocTraits::construct(	storage_.alloc_, 
														storage_.data_ + last, 
														std::move(storage_.data_[last - 1]) );
				++storage_.size_;
				for (; last > idx; --last)
					StorageRAII::AllocTraits::construct(	storage_.alloc_, 
															storage_.data_ + last, 
															std::move(storage_.data_[last - 1]) );
				
			}
			else if 	( 	   !std::is_nothrow_move_constructible_v<T> 
							&& !std::is_trivially_copyable_v<T>)
				for (; last > idx; --last)
					StorageRAII::AllocTraits::construct(	storage_.alloc_, 
															storage_.data_ + last, 
															storage_.data_[last - 1] );
			else
			{
				storage_.data_[last] = storage_.data_[last - 1];
				++storage_.size_;
				--last;
				for (; last > idx; --last)
					storage_.data_[last] = storage_.data_[last - 1];
			}
		} 



		void copy_or_move (StorageRAII& temp, size_t& idx, size_t len)
		{
			if constexpr ( 		std::is_nothrow_move_constructible_v<T> 
							&& !std::is_trivially_copyable_v<T>)
				for ( ; idx < len; ++idx)
					StorageRAII::AllocTraits::construct(	temp.alloc_, 
															temp.data_ + idx, 
															std::move(storage_.data_[idx]) );
			else if 	( 	   !std::is_nothrow_move_constructible_v<T> 
							&& !std::is_trivially_copyable_v<T>)
				for ( ; idx < len; ++idx)
					StorageRAII::AllocTraits::construct(	temp.alloc_, 
															temp.data_ + idx, 
															storage_.data_[idx] );
			else
				for ( ; idx < len; ++idx)
					temp.data_ [idx] = storage_.data_[idx];
		} 


	private:
		template <class V>
		constexpr void push_back_impl(V&& val)
		{
			if (storage_.size_ == storage_.capacity_)
			{
				StorageRAII temp(storage_.size_ * 2 + 1);
				copy_or_move(temp, temp.size_, storage_.size_);
				new (temp.data_ + temp.size_) T(std::forward<V>(val));
				++temp.size_;
				storage_ = std::move(temp);
			}
			else
			{
				new (storage_.data_ + storage_.size_) T(std::forward<V>(val));
				++storage_.size_;
			}
		}
		
	public:
		constexpr void push_back(const T& copy_val) { push_back_impl(copy_val); }
		constexpr void push_back(T&& move_val)  { push_back_impl(move_val); }

		constexpr void reserve(size_t new_cap)
		{
			if (new_cap <= storage_.capacity_)
				return ;
			StorageRAII temp(new_cap);
			copy_or_move(temp, temp.size_, storage_.size_);
			++temp.size_;
			storage_ = std::move(temp);
		}

		void 	swap		(MyVector& rhs) noexcept
		{ 
			storage_ = std::move(rhs.storage_);
		}

}; // end MyVector

} // end namespace "myvec"

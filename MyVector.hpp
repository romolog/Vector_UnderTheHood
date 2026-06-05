#pragma once

#include "StorageRAII.hpp"
#include "MyIterator.hpp"

#include <algorithm> 
#include <cstddef> // ptrdiff_t
#include <type_traits>
#include <concepts>
#include <cstring> // memcpy

// UNDER THE HOOD 
//	it's my self-taught project
//		implementation close to std::vector
//		exception safety
//		using different techniques: C++20/23 and their older analogues 
//		iterator friendly


namespace myvec
{

	// TODO:
	// struct small_vec_allocator

	template <typename T, typename Pointer, typename Reference>
	class MyIterator;

	// template <typename T>
	// class MyConstIterator;

	template <typename T, typename Allocator = std::allocator<T> >
	class MyVector
	{


		// exception safety
		//		destruction of StorageRAII will be called 
		//		even if (copy) constructor of MyVector would fail
		using StorageRAII = myvec::StorageRAII<T, Allocator>;
		StorageRAII storage_;


		public:

		//	nested: noexcept(Allocator()) = BOOL (True or False);
		//	outer: 	noexcept (True) = noexcept
		//			noexcept (False) =  throws
		constexpr MyVector() noexcept(noexcept(Allocator())) : MyVector(Allocator()) {}

		constexpr explicit MyVector(const Allocator& alloc ) : storage_(StorageRAII(0, alloc)) {}

		explicit MyVector(size_t size, const Allocator& alloc = Allocator())
			requires std::default_initializable<T>
			: storage_(StorageRAII(size, alloc))
		{
			for(; storage_.size_ < size; ++storage_.size_)
				new(storage_.data_ + storage_.size_) T();
		}

		constexpr MyVector(size_t count, const T& val, const std::type_identity_t<Allocator>& alloc = Allocator())
			requires std::copy_constructible<T>
			: storage_(StorageRAII(count, alloc))
		{
			for(; storage_.size_ < count; ++storage_.size_)
				new(storage_.data_ + storage_.size_) T(val);
		}

// | Member                                   | Description                                              
// | ---------------------------------------- | -------------------------------------------------------- 
// | `iterator_traits<It>::difference_type`   | Signed integer type used for distances between iterators 
// | `iterator_traits<It>::value_type`        | Type of elements pointed to by the iterator              
// | `iterator_traits<It>::pointer`           | Pointer type to the element                              
// | `iterator_traits<It>::reference`         | Reference type to the element                            
// | `iterator_traits<It>::iterator_category` | Legacy iterator category tag                             
// | `iterator_traits<It>::iterator_concept`  | C++20 iterator concept classification                    


// | Member                                   | Typical Type / Example                                             |
// | ---------------------------------------- | ------------------------------------------------------------------ |
// | `iterator_traits<It>::difference_type`   | `std::ptrdiff_t`                                                   |
// | `iterator_traits<It>::value_type`        | `int`, `std::string`, etc.                                         |
// | `iterator_traits<It>::pointer`           | `T*`                                                               |
// | `iterator_traits<It>::reference`         | `T&`, `const T&`                                                   |
// | `iterator_traits<It>::iterator_category` | `std::input_iterator_tag`, `std::random_access_iterator_tag`, etc. |
// | `iterator_traits<It>::iterator_concept`  | `std::forward_iterator_tag`, `std::contiguous_iterator_tag`, etc.  |



		template <typename It>
		using iterator_category_t = typename std::iterator_traits<It>::iterator_category;

		template <	typename InputIt, 
					typename = std::enable_if_t<	
								std::is_base_of_v<	
									std::input_iterator_tag, iterator_category_t< InputIt> > > >
		constexpr MyVector(InputIt first, InputIt last, const std::type_identity_t<Allocator>& alloc = Allocator()) :
			storage_(StorageRAII(std::distance(first, last), alloc))
		{
			for(; first != last; ++first, ++storage_.size_)
				new(storage_.data_ + storage_.size_) T(*first);
		}

		//	TODO: add range ctor
		// template< container-compatible-range<T> R >
		// constexpr vector( std::from_range_t, R&& rg,
        //           const Allocator& alloc = Allocator() );


		constexpr MyVector(MyVector&& move) noexcept : storage_(std::move(move.storage_)) {}

		constexpr MyVector(MyVector&& move, const std::type_identity_t<Allocator>& alloc) noexcept 
			: storage_(std::move(move.storage_, alloc)) {}

		MyVector& operator=(MyVector&& move) 
		{
			storage_ = std::move(move.storage_);
			return *this;
		}


		// Technique #1 to block a call of Ctor: Conditional_t< Bool, Class, BlockingClass>
		// 		could be replaced by C++20: requires std::is_copy_constructible_v<T>
		private:
			class Blocking {};
		public:
		using IfCopyConstructibleConditional = 
					std::conditional_t<std::is_copy_constructible_v<T>, MyVector, Blocking>;
		constexpr MyVector(const IfCopyConstructibleConditional& copy) : 
			storage_(StorageRAII(copy.storage_.size_))
		{
			// if-constexpr means no branch at runtime
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				std::memcpy(storage_.data_, copy.storage_.data_, copy.storage_.size_ * sizeof(T));
				storage_.size_     = copy.storage_.size_;
			}
			else
			{
				//	FOR EXCEPTION SAFETY uninitialized_copy needs 'noexcept' for CopyCtor of T :
				// 		std::uninitialized_copy(copy.data_, copy.data_ + copy.storage_.size_, data_);
				// 		size_     = copy.storage_.size_;
				//
				//	manual approach is better than uninit_copy for exception safety
				//	size_ is catched properly to call Dtor of StorageRAII:
				for(; storage_.size_ < copy.storage_.size_; ++storage_.size_)
					new(storage_.data_ + storage_.size_) T(copy.storage_.data_[storage_.size_]);
			}
		}

		//	std::type_identity_t = 	prevents guarded parameter from template deduction/substitution
		//							stop implicit conversions
		//							only exact type match
		//			MyVector<T, custom_allocator > 	copy; 
		//				IMPORTANT: custom_allocator can be implicit cast to base_allocator
		//			MyVector<T, base_allocator > mv(copy); = FAIL, impicit cast is blocked
		//				if not blocked, mv will be created with BASE_ALLOCATOR instead of custom_allocator
		//			MyVector<T, custom_allocator > 	m2v(copy); = OK, exact match
		//	PROBLEM CASE: ???
		//		vector(const vector& other, const Alloc& alloc);
		// 		vector(vector&& other, const Alloc& alloc);
		//			without std::type_identity_t<> compiler could make wrong overload
		//	EXPLANATION:
		//		types inside 'type_identity_t' become non-deduced contexts:
		//			template<typename T> void foo(T a, T b);
		//				foo(1, 2.0); = FAIL
		//				foo<int, int>(1, 2.0); = OK, but not cool		
		//			template<typename T> void boo(T a, std::type_identity_t<T> b);
		//				boo(1, 2.0); = OK, deduced a -> int
		//	VS explicit
		//		Historically:
		// 			explicit mattered for constructors callable with one argument.
		// 		Since C++20:
		// 			multi-parameter constructors can syntactically be explicit,
		// 			but it only matters if callable with single argument after defaults.
		//
		//	BEAUTIFUL:
		//		template<class T>
		// 		struct WeirdAlloc {
		// 			template<class U>
		// 			WeirdAlloc(const WeirdAlloc<U>&) {}
		// 		};
		//			EXPLANATION:
		//				without eleminating alloc from class deduction,
		//				MyVector<int, WeirdAlloc> wv();
		//				MyVector v(wv); // FAIL, can't resolve ambiguity
		constexpr MyVector(	const MyVector& copy, 
							const std::type_identity_t<Allocator>& alloc) 
			requires std::copy_constructible<T> 
			: storage_(StorageRAII(copy.storage_.size_, alloc))
		{
			// if-constexpr means no branch at runtime
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				std::memcpy(storage_.data_, copy.storage_.data_, copy.storage_.size_ * sizeof(T));
				storage_.size_     = copy.storage_.size_;
			}
			else
			{
				//	FOR EXCEPTION SAFETY uninitialized_copy needs 'noexcept' for CopyCtor of T :
				// 		std::uninitialized_copy(copy.data_, copy.data_ + copy.storage_.size_, data_);
				// 		size_     = copy.storage_.size_;
				//
				//	manual approach is better than uninit_copy for exception safety
				//	size_ is catched properly to call Dtor of StorageRAII:
				for (; storage_.size_ < copy.storage_.size_; ++storage_.size_)
					new(storage_.data_ + storage_.size_) T(copy.storage_.data_[storage_.size_]);
			}
		}


		// Technique #2 to block a call of Ctor: SFINAE void_t < decltype( T( declval<T>() )) >
		// 		could be replaced by C++20: requires std::is_copy_constructible_v<T>
		template <typename Arg>
		using IfCopyConstructibleSFINAE = std::void_t< decltype( Arg(std::declval<const Arg&>() ) ) >;

		template <class Arg = T, class = IfCopyConstructibleSFINAE<Arg> >
		MyVector& operator=(const MyVector& copy)
		{
			MyVector newCopy(copy);
			*this = std::move(newCopy); 
		}


		//	std::initializer_list is already inherently const — its elements are always const T, 
		//	so adding const to the list itself is redundant.
		MyVector(std::initializer_list<T> init_list, const std::type_identity_t<Allocator>& alloc = Allocator()) 
			requires std::copy_constructible<T>
			: storage_(StorageRAII(init_list.size(), alloc))
		{

			// template<class InputIt, class UnaryFunc>
			// UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f) {
			// 	for (; first != last; ++first)
			// 		f(*first);   // ← dereferences iterator, passes element !!!
			// 	return f;
			// }

			std::for_each(	init_list.begin(), 
							init_list.end(), 
							[this](auto&& val) { 
								StorageRAII::AllocTraits::construct(storage_.alloc_, storage_.data_ + storage_.size_, val);
								// new(storage_.data_ + storage_.size_) T(val); 
								++storage_.size_;
							} );

			// INSTEAD OF:
			//		for (auto it = init_list.begin(), end = init_list.end(); it != end; ++it, ++storage_.size_)
			// 			new(storage_.data_ + storage_.size_) T(*it);
		}


		using iterator = MyIterator<T, T*, T&>;
		using const_iterator = MyIterator<T, const T*, const T&>;
		using reverse_iterator = MyReverseIterator<iterator>;
		using const_reverse_iterator = MyIterator<const_iterator>;

		iterator begin() noexcept { return iterator(storage_.data_); }
		iterator end() noexcept { return iterator(storage_.data_ + storage_.size_); }

		const_iterator begin() const noexcept { return const_iterator(storage_.data_); }
		const_iterator end() const noexcept { return const_iterator(storage_.data_ + storage_.size_); }
		const_iterator cbegin() const noexcept { return const_iterator(storage_.data_); }
		const_iterator cend() const noexcept { return const_iterator(storage_.data_ + storage_.size_); }

		const_iterator rbegin() const noexcept { return reverse_iterator(storage_.data_ + storage_.size_); }
		const_iterator rend() const noexcept { return reverse_iterator(storage_.data_); }
		const_iterator crbegin() const noexcept { return const_reverse_iterator(storage_.data_ + storage_.size_); }
		const_iterator crend() const noexcept { return const_reverse_iterator(storage_.data_); }


		T& operator[](size_t id) noexcept {return storage_.data_[id];}
		const T& operator[](size_t id) const noexcept {return storage_.data_[id];}


//------------------------------------------------------------------------------------------------
		size_t	size		(void)	const	noexcept { return storage_.size_; }

		size_t	capacity	(void)	const	noexcept { return storage_.capacity_; }

		T*		data		(void)	const	noexcept { return storage_.data_; }

//------------------------------------------------------------------------------------------------
		void	clear		(void)	noexcept 
		{ 
			// storage_.~storage(); 
			// storage_ = nullptr;
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
					// new (temp.data_ + idx) T(std::move(storage_.data_[idx]));
					StorageRAII::AllocTraits::construct(	storage_.alloc_, 
															storage_.data_ + last, 
															std::move(storage_.data_[last - 1]) );
				
			}
			else if 	( 	   !std::is_nothrow_move_constructible_v<T> 
							&& !std::is_trivially_copyable_v<T>)
				for (; last > idx; --last)
					StorageRAII::AllocTraits::construct(	temp.alloc_, 
															temp.data_ + last, 
															storage_.data_[last - 1] );
					// new (temp.data_ + idx) T(storage_.data_[idx]);
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
					// new (temp.data_ + idx) T(std::move(storage_.data_[idx]));
					StorageRAII::AllocTraits::construct(	temp.alloc_, 
															temp.data_ + idx, 
															std::move(storage_.data_[idx]) );
			else if 	( 	   !std::is_nothrow_move_constructible_v<T> 
							&& !std::is_trivially_copyable_v<T>)
				for ( ; idx < len; ++idx)
					StorageRAII::AllocTraits::construct(	temp.alloc_, 
															temp.data_ + idx, 
															storage_.data_[idx] );
					// new (temp.data_ + idx) T(storage_.data_[idx]);
			else
				for ( ; idx < len; ++idx)
					temp.data_ [idx] = storage_.data_[idx];
		} 


		private:
		// template<class U>
		// void push_back_impl(U&& value)
		// {
		//     ::new (data_ + size_) T(value);
		// }
		//	IMPORTANT: 
		//		Even though value's type is U&&, the named variable value 
		//		is always an lvalue expression inside the function.
		//		so finally T(value) -> T(const U&) = copy 
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
			// no allocator propagation intentionally
		}


		// insert()
		// back()
		// pop_back()
		// erase()

	}; // end MyVector

	// template <typename Allocator>
	// class MyVector<bool, Allocator>
	// {
	// 	struct StorageRAII
	// 	{
	// 		using AllocTraits = std::allocator_traits<Allocator>;
	// 		Allocator 	alloc_ = Allocator();

	// 		size_t		capacity_	= 0;
			
	// 		using WordType = std::size_t;
	// 		WordType* 	data_		= nullptr;
			
	// 		size_t		size_		= 0;


	// 		StorageRAII(const size_t capacity, const Allocator& some_alloc = Allocator()) :
	// 		alloc_(some_alloc), 
	// 		capacity_(capacity), 
	// 		data_(AllocTraits::allocate(alloc_, capacity)) // = data_ (::operator new(sizeof(T) * capacity))
	// 		{};

	// 	} storage_;
	// };


} // end namespace "myvec"


// PROBLEM:
//		template parameter T deduction failed
// template <typename T, typename Allocator>
// myvec::MyVector<T, Allocator>::iterator 
// operator+( 
// 		typename myvec::MyVector<T, Allocator>::iterator::difference_type shift, 
// 		const typename myvec::MyVector<T, Allocator>::iterator& rhs)
// {
// 	typename myvec::MyVector<T, Allocator>::iterator res {rhs.data_ + shift};
// 	return res;
// };
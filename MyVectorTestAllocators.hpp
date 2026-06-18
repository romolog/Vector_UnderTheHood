#pragma once

#include <atomic>
#include <cstddef>
#include <memory>

namespace myvec
{
	template<class T>
	struct AllocThrow
	{
		using value_type = T;
	
		inline static int allocs_before_throw_ = 0;
	
		T* allocate(std::size_t size)
		{
			--allocs_before_throw_;
			if (allocs_before_throw_ < 0)
				throw std::bad_alloc{};
	
			return static_cast<T*>( ::operator new(sizeof(T) * size) );
		}
	
		void deallocate(T* p, std::size_t)
		{
			::operator delete(p);
		}
	};
	
	template <typename T>
	struct AllocCount
	{
		using value_type = T;
		inline static size_t allocations_ = 0;
		inline static size_t deallocations_ = 0;
		size_t prev_allocs_;
		size_t prev_deallocs_;

		T* allocate(std::size_t n)
		{
			++allocations_;
			return static_cast<T*>( ::operator new(n * sizeof(T)) );
		}
	
		void deallocate(T* p, std::size_t)
		{
			++deallocations_;
			::operator delete(p);
		}

		void reset(void) 
		{
			prev_allocs_ = allocations_;
			prev_deallocs_ = deallocations_;
		}

		size_t allocations(void) { return (allocations_ - prev_allocs_); }
		size_t deallocations(void) { return (deallocations_ - prev_deallocs_); }

	};
} // end namespace myvec

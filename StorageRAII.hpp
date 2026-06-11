#include <memory>

namespace myvec
{

	template <typename T, typename Allocator>
	struct StorageRAII
	{
		using AllocTraits = std::allocator_traits<Allocator>;
		Allocator 	alloc_ = Allocator();
		size_t		capacity_	= 0;
		T* 			data_		= nullptr;
		size_t		size_		= 0;
	
	
	// default vs empty {}
	// | Aspect                                     | `MyVec() = default;`               | `MyVec() {}`                              |
	// | ------------------------------------------ | ---------------------------------- | ----------------------------------------- |
	// | Meaning                                    | Compiler generates				 | User-provided constructor with empty body |
	// | Constructor kind                           | Defaulted                          | User-provided                             |
	// | Triviality possible                        | Yes                                | No                                        |
	// | Can remain trivially default constructible | Yes                                | No                                        |
	// | Implicit `constexpr` friendliness          | Better                             | Worse                                     |
	// | Aggregate status impact                    | Often preserves aggregate rules    | Breaks aggregate status                   |
	// | Optimization opportunities                 | Maximal                            | Potentially reduced                       |
	// | Intent clarity                             | “Use normal compiler behavior”     | “Custom behavior exists”                  |
	// | Rule-of-zero / modern style                | Preferred                          | Usually avoided unless needed             |
	// | Exception specification                    | Automatically deduced optimally    | Different rules for user-provided ctor    |
	// | ABI / low-level type traits                | Preserved better                   | Can change traits                         |
	// | Boilerplate                                | Minimal                            | Unnecessary code                          |
	// | Recommended when no custom logic needed    | Yes                                | No                                        |
	
	
		StorageRAII() = default;
	
		// StorageRAII(Allocator some_alloc = Allocator()) :
		// 	alloc_(some_alloc) 
		// 	capacity_(capacity),
		// 	data_(AllocTraits_::allocate(alloc_, capacity)) // = data_ (::operator new(sizeof(T) * capacity))
		// 	// size_ (0) added by compiler
		// {};
	
		StorageRAII(size_t capacity, const Allocator& some_alloc = Allocator()) :
			alloc_(some_alloc), 
			capacity_(capacity), 
			data_(AllocTraits::allocate(alloc_, capacity)) // = data_ (::operator new(sizeof(T) * capacity))
			// size_ (0) added by compiler
		{}
		
		StorageRAII(const StorageRAII&) = delete; 
		StorageRAII& operator=(const StorageRAII&) = delete;
		// StorageRAII(const StorageRAII& copy) 
		// {}

		// StorageRAII& operator=(const StorageRAII& copy) 
		// {return *this;}


		StorageRAII(StorageRAII&& move) noexcept
		{
			// allocator is not propagated intentionally 
			std::swap(capacity_, move.capacity_);
			std::swap(data_, move.data_);
			std::swap(size_, move.size_);
		}
	
		StorageRAII& operator=(StorageRAII&& move) noexcept
		{
			// allocator is not propagated intentionally
			std::swap(capacity_, move.capacity_);
			std::swap(data_, move.data_);
			std::swap(size_, move.size_);
			return *this;
		}
	
		~StorageRAII()
		{
			for(size_t i = 0; i < size_; ++i)
				data_[i].~T();
				// AllocTraits::destroy(data_ + i); // data_[i].~T();
			
			// custom allocator may not support deallocate(custom_alloc, nullptr, 0)
			if (data_)
				AllocTraits::deallocate(alloc_, data_, capacity_);
		}
	
		//	alignas alignof std::align_val_t
		// ::operator new(n * sizeof(T), std::align_val_t(alignof(T)))
		//		std::align_val_t is a strongly typed enum introduced in C++17 
		//		that represents an alignment value — used to pass alignment to operator new in a type-safe way.
		// Deallocate — must use matching aligned delete
		// 		::operator delete(p, std::align_val_t(64));
	
		// ✅ Cast size_t to align_val_t
		// std::align_val_t al = std::align_val_t(64);
	
		// ✅ Cast back to size_t
		// size_t n = static_cast<size_t>(al);
	
		// ❌ Implicit conversion not allowed
		// std::align_val_t al = 64;   // error — no implicit conversion
	};
}
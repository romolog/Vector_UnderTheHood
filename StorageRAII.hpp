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
	
		StorageRAII() = default;
	
	
		StorageRAII(size_t capacity, const Allocator& some_alloc = Allocator()) :
			alloc_(some_alloc), 
			capacity_(capacity)
		{
			data_ = AllocTraits::allocate(alloc_, capacity); 
		}
		
		StorageRAII(const StorageRAII&) = delete; 
		StorageRAII& operator=(const StorageRAII&) = delete;


		StorageRAII(StorageRAII&& move) noexcept
		{
			std::swap(capacity_, move.capacity_);
			std::swap(data_, move.data_);
			std::swap(size_, move.size_);
		}
	
		StorageRAII& operator=(StorageRAII&& move) noexcept
		{
			std::swap(capacity_, move.capacity_);
			std::swap(data_, move.data_);
			std::swap(size_, move.size_);
			return *this;
		}
	
		~StorageRAII()
		{
			for(size_t i = 0; i < size_; ++i)
				AllocTraits::destroy(alloc_, data_ + i);
			
			if (data_)
				AllocTraits::deallocate(alloc_, data_, capacity_);
		}
	}; // end StorageRAII
} // end myvec namespace

// using is_always_equal = std::true_type;
// Tests allocator-aware optimizations.

// propagate_on_container_copy_assignment
// propagate_on_container_move_assignment
// select_on_container_copy_construction()
template<class T>
struct StatefulAlloc
{
    using value_type = T;

    int id;

    StatefulAlloc(int id = 0)
        : id(id)
    {
    }

    template<class U>
    StatefulAlloc(const StatefulAlloc<U>& other)
        : id(other.id)
    {
    }

    T* allocate(std::size_t n)
    {
        return static_cast<T*>(
            ::operator new(n * sizeof(T))
        );
    }

    void deallocate(T* p, std::size_t)
    {
        ::operator delete(p);
    }
};


// EXPECT_THROW(
// 	vec.push_back(x),
// 	std::bad_alloc
// );
template<class T>
struct ThrowingAlloc
{
    using value_type = T;

    inline static int allocs_before_throw = 0;

    T* allocate(std::size_t n)
    {
        if (--allocs_before_throw < 0)
            throw std::bad_alloc{};

        return static_cast<T*>(
            ::operator new(n * sizeof(T))
        );
    }

    void deallocate(T* p, std::size_t)
    {
        ::operator delete(p);
    }
};


//	EXPECT_EQ(	CountingAlloc<int>::allocations,
//				CountingAlloc<int>::deallocations);
template<class T>
struct CountingAlloc
{
    using value_type = T;

    inline static std::size_t allocations = 0;
    inline static std::size_t deallocations = 0;

    T* allocate(std::size_t n)
    {
        ++allocations;
        return static_cast<T*>(
            ::operator new(n * sizeof(T))
        );
    }

    void deallocate(T* p, std::size_t)
    {
        ++deallocations;
        ::operator delete(p);
    }
};
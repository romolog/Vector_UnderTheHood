#pragma once
#include <type_traits>
#include <concepts>
#include <iterator>


template<std::input_iterator T, std::input_iterator U>
struct PairInputIters
{
	T first;
	U second;

	constexpr PairInputIters& operator++() noexcept
	{
		++first;
		++second;
		return *this;
	}

	constexpr bool operator!=(const PairInputIters& other) const noexcept 
	{ 
		return first != other.first || second != other.second;
	}
};

// this to check number of copies of elements
struct Counter
{
	//	inline allows to define in header
	//	I still can change variable
    static inline int copy_ctor = 0;

    Counter() = default;

    Counter(const Counter&) { ++copy_ctor; }
};


class ThrowClass
{
	private:
		static int count_;
		char* ptr;

	public:
		ThrowClass()
		{
			++count_;
			// std::cout << "ThrowClass Default Ctor: " << count_ << "\n";
			if (count_ % 3)
				throw std::runtime_error("ThrowClass Default Ctor");
			ptr = new char[42];
		};
		~ThrowClass(){ delete[] ptr;};
};
#include "MyVector.hpp"

#include <random>
#include <string>
#include <vector>
#include <type_traits>
#include <concepts>


// GTEST_LOG_(INFO) << "\n\tCall default ctor, check if size = 0, empty = true\n";
// EXPECT_NE(t.data(), nullptr); // ASSERT_NE(ptr, nullptr) = stop if fail


template<class Vec>
void TestSizeVector(Vec& t, size_t size)
{

	EXPECT_EQ(t.size(), size);
	EXPECT_GE(t.capacity(), size);
	
	if (size)
	{
		EXPECT_NE(t.data(), nullptr);
		EXPECT_FALSE(t.empty());
		EXPECT_NE(t.begin(), t.end());
		EXPECT_NE(t.cbegin(), t.cend());
		EXPECT_NE(t.rbegin(), t.rend());
		EXPECT_NE(t.crbegin(), t.crend());
	}
	else
		EXPECT_TRUE(t.empty());

	EXPECT_EQ(t.end() - t.begin(), size);
	EXPECT_EQ(t.cend() - t.cbegin(), size);
	EXPECT_EQ(t.rend() - t.rbegin(), size);
	EXPECT_EQ(t.crend() - t.crbegin(), size);

	int count = 0;
	for (auto&& _ : t)
	{
		(void)_;
		++count;
	}
	EXPECT_EQ(count, size);
};

template<typename Vec>
void TestSizeValVector(Vec& t)
{
	TestSizeVector(t, 0);
};

template<typename Vec>
void TestSizeValVector(Vec& t, size_t size, const typename Vec::value_type& val)
{

	TestSizeVector(t, size);

	std::for_each(t.begin(), t.end(), [val](auto&& el){EXPECT_EQ(el, val);});
	// EXPECT_EQ(t[0], val);

};

template<typename Vec, typename Iter>
	requires std::input_iterator<Iter>
void TestSizeValVector(Vec& t, Iter it, Iter end)
{

	TestSizeVector(t, std::distance(it, end));

	std::for_each(t.begin(), t.end(), [&it, &end](auto&& el){EXPECT_EQ(el, *it++);});

};

template <typename T>
std::vector<T> std_vector42_shuffled(void)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::vector<int> v(42);
	std::iota(v.begin(), v.end(), 0);
	std::shuffle(v.begin(), v.end(), gen);
	
	if (std::is_same_v<T, int>)
		return v;
	else if (std::is_same_v<T, std::__cxx11::basic_string<char> > )
	{	
		auto iter = v.begin();
		std::uniform_int_distribution<int> i_dist(0, 25);				
		
		auto make_random_string = [&iter, &i_dist, &gen]()
		{
			size_t len = *iter;
			++iter; 
			std::string str;
			str.reserve(len);

			char rand_ch = i_dist(gen);
			str.push_back('A' + rand_ch);
			
			auto make_random_lowercase = [&i_dist, &gen]()
			{
				char rand_ch = i_dist(gen);
				return ('a' + rand_ch);
			};

			std::generate(str.begin() + 1, str.end(), make_random_lowercase);

			return str;
		};

		std::vector<T> vt(42);
		std::generate(vt.begin(), vt.end(), make_random_string);

		return vt;
	}
	else
	{
		std::vector<T> vt(42);
		auto iter = v.begin();
		std::generate(vt.begin(),vt.end(),[&iter](){ T(*iter++); });
		return vt;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

template<class Vec>
void TestEmptyVector(Vec& t)
{
	EXPECT_EQ(t.size(), 0);
	EXPECT_EQ(t.capacity(), 0);
	EXPECT_TRUE(t.empty());
	EXPECT_EQ(t.begin(), t.end());
	EXPECT_EQ(t.cbegin(), t.cend());
	EXPECT_EQ(t.rbegin(), t.rend());
	EXPECT_EQ(t.crbegin(), t.crend());

	int count = 0;
	for (auto&& _ : t)
	{
		(void)_;
		++count;
	}
	EXPECT_EQ(count, 0);
};

template <class T>
void RunEmptyVectorTests (void) 
{	
	// VALIDATE TEST ON STD LIB
	std::vector<T> st;
	TestEmptyVector(st);

	// EMPTY VECTOR
	myvec::MyVector<T> t;
	TestEmptyVector(t);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T> ct;
	TestEmptyVector(ct);
	// ct.clear(); // OK : compile error

	// COPY
	myvec::MyVector<T> src_mutable_copy;
	TestEmptyVector(src_mutable_copy);
	const myvec::MyVector<T> const_copy{src_mutable_copy};
	TestEmptyVector(const_copy);

	// COPY FROM CONST
	const myvec::MyVector<T> src_const_copy;
	TestEmptyVector(src_const_copy);
	myvec::MyVector<T> mutable_copy{src_const_copy};
	TestEmptyVector(mutable_copy);

	// MOVE
	myvec::MyVector<T> src_move_mutable;
	TestEmptyVector(src_move_mutable);
	myvec::MyVector<T> move{std::move(src_move_mutable)};
	TestEmptyVector(move);

	// MOVE(->COPY) FROM CONST
	const myvec::MyVector<T> src_const_refref;
	TestEmptyVector(src_const_refref);
	myvec::MyVector<T> copy_from_const_refref{std::move(src_const_refref)};
	TestEmptyVector(copy_from_const_refref);

	// COPY ASSIGN
	myvec::MyVector<T> src_copy_assign;
	TestEmptyVector(src_copy_assign);	
	myvec::MyVector<T> copy_assign;
	TestEmptyVector(copy_assign);
	copy_assign = src_copy_assign;
	TestEmptyVector(copy_assign);

	// COPY ASSIGN FROM CONST
	const myvec::MyVector<T> src_const_copy_assign;
	TestEmptyVector(src_const_copy_assign);	
	myvec::MyVector<T> assign_copy_from_const;
	TestEmptyVector(assign_copy_from_const);
	assign_copy_from_const = src_const_copy_assign;
	TestEmptyVector(assign_copy_from_const);

	// COPY ASSIGN SELF
	myvec::MyVector<T> copy_self;
	TestEmptyVector(copy_self);
	copy_self = copy_self;
	TestEmptyVector(copy_self);

	// MOVE ASSIGN
	myvec::MyVector<T> src_move_assign;
	TestEmptyVector(src_move_assign);	
	myvec::MyVector<T> move_assign;
	TestEmptyVector(move_assign);
	move_assign = std::move(src_move_assign);
	TestEmptyVector(move_assign);

	// MOVE ASSIGN SELF
	myvec::MyVector<T> move_self;
	TestEmptyVector(move_self);
	move_self = std::move(move_self);
	TestEmptyVector(move_self);
}

template <class T>
void RunZeroVectorTests (void) 
{	
	// VALIDATE TEST ON STD LIB
	std::vector<T> st(0);
	TestEmptyVector(st);

	// EMPTY VECTOR
	myvec::MyVector<T> t(0);
	TestEmptyVector(t);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T> ct(0);
	TestEmptyVector(ct);
	// ct.clear(); // OK : compile error

	// COPY
	myvec::MyVector<T> src_mutable_copy(0);
	TestEmptyVector(src_mutable_copy);
	const myvec::MyVector<T> const_copy{src_mutable_copy};
	TestEmptyVector(const_copy);

	// COPY FROM CONST
	const myvec::MyVector<T> src_const_copy(0);
	TestEmptyVector(src_const_copy);
	myvec::MyVector<T> mutable_copy{src_const_copy};
	TestEmptyVector(mutable_copy);

	// MOVE
	myvec::MyVector<T> src_move_mutable(0);
	TestEmptyVector(src_move_mutable);
	myvec::MyVector<T> move{std::move(src_move_mutable)};
	TestEmptyVector(move);

	// MOVE(->COPY) FROM CONST
	const myvec::MyVector<T> src_const_refref(0);
	TestEmptyVector(src_const_refref);
	myvec::MyVector<T> copy_from_const_refref{std::move(src_const_refref)};
	TestEmptyVector(copy_from_const_refref);

	// COPY ASSIGN
	myvec::MyVector<T> src_copy_assign(0);
	TestEmptyVector(src_copy_assign);	
	myvec::MyVector<T> copy_assign(0);
	TestEmptyVector(copy_assign);
	copy_assign = src_copy_assign;
	TestEmptyVector(copy_assign);

	// COPY ASSIGN FROM CONST
	const myvec::MyVector<T> src_const_copy_assign(0);
	TestEmptyVector(src_const_copy_assign);	
	myvec::MyVector<T> assign_copy_from_const(0);
	TestEmptyVector(assign_copy_from_const);
	assign_copy_from_const = src_const_copy_assign;
	TestEmptyVector(assign_copy_from_const);

	// COPY ASSIGN SELF
	myvec::MyVector<T> copy_self(0);
	TestEmptyVector(copy_self);
	copy_self = copy_self;
	TestEmptyVector(copy_self);

	// MOVE ASSIGN
	myvec::MyVector<T> src_move_assign(0);
	TestEmptyVector(src_move_assign);	
	myvec::MyVector<T> move_assign(0);
	TestEmptyVector(move_assign);
	move_assign = std::move(src_move_assign);
	TestEmptyVector(move_assign);

	// MOVE ASSIGN SELF
	myvec::MyVector<T> move_self(0);
	TestEmptyVector(move_self);
	move_self = std::move(move_self);
	TestEmptyVector(move_self);
}

template <class T>
void RunSizeVectorTests (size_t size) 
{	
	ASSERT_GT(size, 0);

	// VALIDATE TEST ON STD LIB
	std::vector<T> st(size);
	TestSizeVector(st, size);

	// EMPTY VECTOR
	myvec::MyVector<T> t(size);
	TestSizeVector(t, size);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T> ct(size);
	TestSizeVector(ct, size);
	// ct.clear(); // OK : compile error

	// COPY
	myvec::MyVector<T> src_mutable_copy(size);
	TestSizeVector(src_mutable_copy, size);
	const myvec::MyVector<T> const_copy{src_mutable_copy};
	TestSizeVector(const_copy, size);

	// COPY FROM CONST
	const myvec::MyVector<T> src_const_copy(size);
	TestSizeVector(src_const_copy, size);
	myvec::MyVector<T> mutable_copy{src_const_copy};
	TestSizeVector(mutable_copy, size);

	// MOVE
	myvec::MyVector<T> src_move_mutable(size);
	TestSizeVector(src_move_mutable, size);
	myvec::MyVector<T> move{std::move(src_move_mutable)};
	TestSizeVector(move, size);

	// MOVE(->COPY) FROM CONST
	const myvec::MyVector<T> src_const_refref(size);
	TestSizeVector(src_const_refref, size);
	myvec::MyVector<T> copy_from_const_refref{std::move(src_const_refref)};
	TestSizeVector(copy_from_const_refref, size);

	// COPY ASSIGN
	myvec::MyVector<T> src_copy_assign(size);
	TestSizeVector(src_copy_assign, size);	
	myvec::MyVector<T> copy_assign(size);
	TestSizeVector(copy_assign, size);
	copy_assign = src_copy_assign;
	TestSizeVector(copy_assign, size);

	// COPY ASSIGN FROM CONST
	const myvec::MyVector<T> src_const_copy_assign(size);
	TestSizeVector(src_const_copy_assign, size);	
	myvec::MyVector<T> assign_copy_from_const(size);
	TestSizeVector(assign_copy_from_const, size);
	assign_copy_from_const = src_const_copy_assign;
	TestSizeVector(assign_copy_from_const, size);

	// COPY ASSIGN SELF
	myvec::MyVector<T> copy_self(size);
	TestSizeVector(copy_self, size);
	copy_self = copy_self;
	TestSizeVector(copy_self, size);

	// MOVE ASSIGN
	myvec::MyVector<T> src_move_assign(size);
	TestSizeVector(src_move_assign, size);	
	myvec::MyVector<T> move_assign(size);
	TestSizeVector(move_assign, size);
	move_assign = std::move(src_move_assign);
	TestSizeVector(move_assign, size);

	// MOVE ASSIGN SELF
	myvec::MyVector<T> move_self(size);
	TestSizeVector(move_self, size);
	move_self = std::move(move_self);
	TestSizeVector(move_self, size);
}

template <class T>
void RunSizeValVectorTests (size_t size, T value) 
{	
	// VALIDATE TEST ON STD LIB
	std::vector<T> st(size, value);
	TestSizeValVector(st, size, value);

	// EMPTY VECTOR
	myvec::MyVector<T> t(size, value);
	TestSizeValVector(t, size, value);

	// EMPTY VECTOR CONST
	const myvec::MyVector<T> ct(size, value);
	TestSizeValVector(ct, size, value);
	// ct.clear(); // OK : compile error

	// COPY
	myvec::MyVector<T> src_mutable_copy(size, value);
	TestSizeValVector(src_mutable_copy, size, value);
	const myvec::MyVector<T> const_copy{src_mutable_copy};
	TestSizeValVector(const_copy, size, value);

	// COPY FROM CONST
	const myvec::MyVector<T> src_const_copy(size, value);
	TestSizeValVector(src_const_copy, size, value);
	myvec::MyVector<T> mutable_copy{src_const_copy};
	TestSizeValVector(mutable_copy, size, value);

	// MOVE
	myvec::MyVector<T> src_move_mutable(size, value);
	TestSizeValVector(src_move_mutable, size, value);
	myvec::MyVector<T> move{std::move(src_move_mutable)};
	TestSizeValVector(move, size, value);

	// MOVE(->COPY) FROM CONST
	const myvec::MyVector<T> src_const_refref(size, value);
	TestSizeValVector(src_const_refref, size, value);
	myvec::MyVector<T> copy_from_const_refref{std::move(src_const_refref)};
	TestSizeValVector(copy_from_const_refref, size, value);

	// COPY ASSIGN
	myvec::MyVector<T> src_copy_assign(size, value);
	TestSizeValVector(src_copy_assign, size, value);	
	myvec::MyVector<T> copy_assign(size, value);
	TestSizeValVector(copy_assign, size, value);
	copy_assign = src_copy_assign;
	TestSizeValVector(copy_assign, size, value);

	// COPY ASSIGN FROM CONST
	const myvec::MyVector<T> src_const_copy_assign(size, value);
	TestSizeValVector(src_const_copy_assign, size, value);	
	myvec::MyVector<T> assign_copy_from_const(size, value);
	TestSizeValVector(assign_copy_from_const, size, value);
	assign_copy_from_const = src_const_copy_assign;
	TestSizeValVector(assign_copy_from_const, size, value);

	// COPY ASSIGN SELF
	myvec::MyVector<T> copy_self(size, value);
	TestSizeValVector(copy_self, size, value);
	copy_self = copy_self;
	TestSizeValVector(copy_self, size, value);

	// MOVE ASSIGN
	myvec::MyVector<T> src_move_assign(size, value);
	TestSizeValVector(src_move_assign, size, value);	
	myvec::MyVector<T> move_assign(size, value);
	TestSizeValVector(move_assign, size, value);
	move_assign = std::move(src_move_assign);
	TestSizeValVector(move_assign, size, value);

	// MOVE ASSIGN SELF
	myvec::MyVector<T> move_self(size, value);
	TestSizeValVector(move_self, size, value);
	move_self = std::move(move_self);
	TestSizeValVector(move_self, size, value);
}



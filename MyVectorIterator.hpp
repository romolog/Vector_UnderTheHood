namespace myvec
{
	class iterator final
	{
		private:
			// MyVector may access inside Iterator;
			//		need it to call iter ctor inside MyVector::begin();
			friend MyVector; 

			T* dataPtr_;

			iterator(T* ptr) noexcept : dataPtr_(ptr) {}

		public:

			using value_type 		= T;
			using difference_type	= std::ptrdiff_t;
			using pointer 			= T*;
			using reference 		= T&;
			using iterator_category = std::random_access_iterator_tag;
			using iterator_concept 	= std::contiguous_iterator_tag;

			iterator(iterator&& move) = default;
			iterator& operator=(iterator&& move) = default;	

			iterator(const iterator& copy) = default;
			iterator& operator=(const iterator& copy) = default;

			T& operator*() const noexcept { return *dataPtr_;}
			T& operator[](const difference_type shift) const noexcept { return *(dataPtr_ + shift);}

			iterator& operator++() noexcept 
			{
				++dataPtr_;
				return *this;
			}

			iterator operator++(int) noexcept 
			{
				iterator prev(*this);
				++dataPtr_;
				return prev;
			}

			iterator& operator--() noexcept 
			{
				--dataPtr_;
				return *this;
			}

			//	just for fun, fragile 
			//	for multiple fileds better make explicity snapshot the state
			//		MyClass prev(*this);
			iterator operator--(int) noexcept { return iterator(dataPtr_--); }

			bool operator<=>(const iterator& second) const noexcept = default; 
			// automatically: == != > >= < <=

			iterator& operator+=(const difference_type shift)
			{	
				dataPtr_ += shift;
				return *this;
			}

			// +n = copy + advance(n)
			// operator+ : implemented by operator+=
			iterator operator+(const difference_type shift)
			{	
				iterator res(*this);
				res += shift;
				return res;
			}
			// IMPORTANT: support num + iter: 
			friend 
			iterator operator+(difference_type shift, iterator it) { return (it += shift);}

			iterator& operator-=(const difference_type shift)
			{	
				dataPtr_ -= shift;
				return *this;
			}

			iterator operator-(const difference_type shift)
			{	
				iterator res(*this);
				res -= shift;
				return res;
			}
			
			difference_type operator-(const iterator& rhs) { return (dataPtr_ - rhs.dataPtr_); }

	};
}
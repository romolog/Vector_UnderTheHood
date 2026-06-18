#pragma once

#include <atomic>
#include <concepts>
#include <iterator>
#include <type_traits>

namespace myvec
{	
	struct AnyCtorAssignThrow final
	{
		AnyCtorAssignThrow	() { throw std::runtime_error("AnyCtorAssignThrow: Default Ctor"); };
	
		AnyCtorAssignThrow	(int) { throw std::runtime_error("AnyCtorAssignThrow: Parametrized Ctor"); };
	
		AnyCtorAssignThrow	(int, int) {}; // special Ctor to instantiate an object, which passed as argument
	
		AnyCtorAssignThrow	(AnyCtorAssignThrow&&) { throw std::runtime_error("AnyCtorAssignThrow: Move Ctor"); };
	
		AnyCtorAssignThrow	operator=(AnyCtorAssignThrow&&) { throw std::runtime_error("AnyCtorAssignThrow: Move Assign"); };
	
		AnyCtorAssignThrow	(const AnyCtorAssignThrow&) { throw std::runtime_error("AnyCtorAssignThrow: Copy Ctor"); };
	
		AnyCtorAssignThrow&	operator=(const AnyCtorAssignThrow&) { throw std::runtime_error("AnyCtorAssignThrow: Copy Assign"); };
		
		~AnyCtorAssignThrow() {};
	};
	
	struct CtorAssignCounter final
	{
		static inline int default_ctor_ = 0;
		static inline int parametrized_ctor_ = 0;
		static inline int copy_ctor_ = 0;
		static inline int copy_assign_ = 0;
		static inline int move_ctor_ = 0;
		static inline int move_assign_ = 0;
		static inline int total_ = 0;
		static inline int dtor_ = 0;
		static inline int created_ = 0;
		
		int this_alive_ = 0;
		int key_ = 0;
	
		static void reset(void) 
		{
			default_ctor_ = 0;
			parametrized_ctor_ = 0;
			copy_ctor_ = 0;
			copy_assign_ = 0;
			move_ctor_ = 0;
			move_assign_ = 0;
			total_ = 0;
			dtor_ = 0;
			created_ = 0;
		}
	
		CtorAssignCounter	() noexcept
							{ ++default_ctor_;		created();	total_update(); }
	
		CtorAssignCounter	(int key) noexcept									
							{ ++parametrized_ctor_; created();	total_update(); key_ = key; }
	
		CtorAssignCounter	(int key, int) noexcept	
							{ key_ = key; } // special Ctor to instantiate for the func arg
	
		CtorAssignCounter	(const CtorAssignCounter& rhs) noexcept				
							{ ++copy_ctor_; 		created();	total_update();  key_ = rhs.key_; }
	
		CtorAssignCounter&	operator=(const CtorAssignCounter& rhs) noexcept	
							{ ++copy_assign_; 		created();	total_update();  key_ = rhs.key_; return *this; }
	
		CtorAssignCounter	(CtorAssignCounter&& rhs) noexcept					
							{ ++move_ctor_; 		created();	total_update();  key_ = rhs.key_; }
	
		CtorAssignCounter	operator=(CtorAssignCounter&& rhs) noexcept			
							{ ++move_assign_; 		created();	total_update();  key_ = rhs.key_; return *this; }	
		
		~CtorAssignCounter	()	{ ++dtor_; }
	
	
		auto operator<=>(const CtorAssignCounter&) const = default;
	
		void created(void)
		{
			if (!this_alive_)
			{
				++this_alive_;
				++created_;	
			}
		}
	
		void total_update(void) 
		{ 
			total_ =  default_ctor_ 
					+ parametrized_ctor_
					+ copy_ctor_
					+ copy_assign_
					+ move_ctor_
					+ move_assign_ ;
		}	
	};
	

	struct NoMove final
	{
		static inline int dtor_ = 0;
		static inline int created_ = 0;
		
		int this_alive_ = 0;
	
		int key_ = 0;
	
		static void reset(void) 
		{
			dtor_ = 0;
			created_ = 0;
		}
	
		NoMove	() noexcept								{ created(); }
		NoMove	(int key) noexcept						{ created(); key_ = key; }
		NoMove	(int key, int) noexcept					{ created(); key_ = key; }
		NoMove	(const NoMove& rhs) noexcept			{ created(); key_ = rhs.key_; }
		NoMove&	operator=(const NoMove& rhs) noexcept	{ created(); key_ = rhs.key_; return *this;}
		NoMove	(NoMove&&)								= delete;		
		NoMove	operator=(NoMove&&)						= delete;	
		~NoMove	()										{ ++dtor_; }
	
		void created(void)
		{
			if (!this_alive_)
			{
				++this_alive_;
				++created_;
			}
		}
	
		auto operator<=>(const NoMove&) const = default;
	};

	
	struct NoCopy final
	{
		static inline int dtor_ = 0;
		static inline int created_ = 0;
		
		int this_alive_ = 0;
	
		int key_ = 0;
	
		static void reset(void) 
		{
			dtor_ = 0;
			created_ = 0;
		}
	
		NoCopy	() noexcept								{ created(); }
		NoCopy	(int key) noexcept						{ created(); key_ = key; }
		NoCopy	(int key, int) noexcept					{ created(); key_ = key; }
		NoCopy	(const NoCopy& rhs) noexcept			{ created(); key_ = rhs.key_; }
		NoCopy&	operator=(const NoCopy& rhs) noexcept	{ created(); key_ = rhs.key_; return *this;}
		NoCopy	(NoCopy&&)								= delete;		
		NoCopy	operator=(NoCopy&&)						= delete;	
		~NoCopy	()										{ ++dtor_; }
	
		void created(void)
		{
			if (!this_alive_)
			{
				++this_alive_;
				++created_;
			}
		}
	
		auto operator<=>(const NoCopy&) const = default;
	};
	
	
	struct StructThrow
	{
		static inline int copies_moves_before_throw_ = 7;
		static inline int created_ = 0;
		char*	ptr_ = nullptr; // this will help to check memory leaks
		int		key_ = 0;
	
		struct Exception : public std::runtime_error 
		{
			Exception(std::string msg) : std::runtime_error(std::move(msg)) {}
		};


		StructThrow() 
		{ 
			ptr_ = new char[42]; 
			++created_; 
			key_ = created_; 
		}

		StructThrow(const StructThrow&)
		{
			--copies_moves_before_throw_;
			if (copies_moves_before_throw_ < 0)
				throw Exception("StructThrow Copy Ctor : " + std::to_string(copies_moves_before_throw_));
			ptr_ = new char[42];
			++created_; 
			key_ = created_;
		}

		StructThrow& operator=(const StructThrow&)
		{
			--copies_moves_before_throw_;
			if (copies_moves_before_throw_ < 0)
				throw Exception("StructThrow Copy Assign : " + std::to_string(copies_moves_before_throw_));
			if (!ptr_)
			{
				ptr_ = new char[42];
				++created_; 
				key_ = created_;
			}
			return *this;
		}

		StructThrow(StructThrow&&)
		{
			--copies_moves_before_throw_;
			if (copies_moves_before_throw_ < 0)
				throw Exception("StructThrow Move Ctor : " + std::to_string(copies_moves_before_throw_));
			ptr_ = new char[42];
			++created_; 
			key_ = created_;
		}

		StructThrow& operator=(StructThrow&&)
		{
			--copies_moves_before_throw_;
			if (copies_moves_before_throw_ < 0)
				throw Exception("StructThrow Move Assign : " + std::to_string(copies_moves_before_throw_));
			if (!ptr_)
			{
				ptr_ = new char[42];
				++created_; 
				key_ = created_;
			}
			return *this;
		}

		~StructThrow()
		{ 
			delete[] ptr_;
		}

		bool operator==(const StructThrow& rhs) const noexcept
		{
			return key_ == rhs.key_;
		}

		int key(void) { return key_; };
	};
} // end namespace myvec

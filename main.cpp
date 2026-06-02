#include "MyVector.hpp"
#include <iostream>

void myprint(size_t n)
{ std::cout << "N = " << n << "\n";}

void myprint_ptr(size_t* ptr)
{ std::cout << "ptr N = " << *ptr << "\n";}

void myprint_cptr(const size_t* cptr)
{ std::cout << "ptr N = " << *cptr << "\n";}

int main(void)
{
	const size_t cn = 42;
	size_t n = 24;
	const size_t* cptr = &cn;
	// ptr = &m; // valid
	// size_t* ptr = &cn; // compile error
	size_t* ptr = &n;

	myprint(n);
	myprint_cptr(cptr);
	myvec::MyVector<int> v(n);
	std::cout << "Size: " << v.size() << "\n";
	return 0;
}
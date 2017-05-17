#include "Blob.h"
#include "BlobPtr.h"
#include "ConstBlobPtr.h"
#include <iostream>
#include <cassert>

template<typename T>
void testElementAccess(Blob<T> obj){
	std::cout << "______________TEST ELEMENT ACCESS____________________" << std::endl;
	auto sz = obj.size();
	std::cout << "Try random access with operator[] and at()" << std::endl;
	for (unsigned i = 0; i!=sz; ++i)
		std::cout << obj[i] << obj.at(i) << " ";
	std::cout << std::endl;

	std::cout << "Try random access on constant objects with operator[] and at()" << std::endl;
	const auto cnst = obj;	// convert to constant
	for (unsigned i = 0; i!=sz; ++i)
		std::cout << cnst[i] << cnst.at(i) << " ";
	std::cout << std::endl;

	std::cout << "print front and back, const and non-const, check front element access"	<< std::endl;
	std::cout << obj.front() << cnst.front() << " " << obj.back() << cnst.back() << std::endl;
	assert((obj.front() = T())==T());
	//assert((cnst.front() = T())==T()); //error: should not be able to change const
	std::cout << "check relational operators" << std::endl;
	std::cout << "==" << std::boolalpha << (obj == Blob<T>(obj)) << " ";
	std::cout << "!=" << std::boolalpha << (obj != cnst) << " ";
	std::cout << "<" << std::boolalpha << (obj < cnst) << " ";
	std::cout << ">" << std::boolalpha << (obj > cnst) << " ";
	std::cout << "<=" << std::boolalpha << (obj <= cnst) << " ";
	std::cout << ">=" << std::boolalpha << (obj >= cnst) << std::endl;
	// TODO: check relational operatos
	std::cout << "check element wise delete with operator[]" << std::endl;
	for (unsigned i = 0; i!=sz; ++i)
		assert((obj[i] = T()) == T());
	std::cout << "check element wise delete again with at() " << std::endl;
	auto copy2 = cnst;	// discards const qualifier
	for (unsigned i = 0; i!=sz; ++i)
		assert((obj.at(i) = T()) == T());
	

}
template<typename T>
void testPointerAccess(Blob<T>& obj){
	std::cout << "______________TEST POITNER ACCESS____________________" << std::endl;
	std::cout << "print elements with BlobPtr" << std::endl;
	for (auto it = obj.begin(); it != obj.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "print elements with ConstBlobPtr" << std::endl;
	for (auto it = obj.cbegin(); it != obj.cend(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
	BlobPtr<T> ptr(obj), ptr2(obj);
	ConstBlobPtr<T> cptr(obj), cptr2(obj);
	std::cout << "check relational operators with BlobPtr" << std::endl;
	std::cout << "==" << std::boolalpha << (ptr == ptr2) << " ";
	std::cout << "!=" << std::boolalpha << (ptr != ptr2) << " ";
	std::cout << "<" << std::boolalpha << (ptr < ptr2) << " ";
	std::cout << ">" << std::boolalpha << (ptr > ptr2) << " ";
	std::cout << "<=" << std::boolalpha << (ptr <= ptr2) << " ";
	std::cout << ">=" << std::boolalpha << (ptr >= ptr2) << std::endl;
	std::cout << "check relational operators with ConstBlobPtr" << std::endl;
	std::cout << "==" << std::boolalpha << (cptr == cptr2) << " ";
	std::cout << "!=" << std::boolalpha << (cptr != cptr2) << " ";
	std::cout << "<" << std::boolalpha << (cptr < cptr2) << " ";
	std::cout << ">" << std::boolalpha << (cptr > cptr2) << " ";
	std::cout << "<=" << std::boolalpha << (cptr <= cptr2) << " ";
	std::cout << ">=" << std::boolalpha << (cptr >= cptr2) << std::endl;
}
int main(){
	Blob<std::string> strBlob{{"Hello","World","!"}};	// explicit template parameter needed (until C++17)
	Blob<int> intBlob{{0, 1,2,3,4,5,6,7,8,9}};
	std::cout << "1) test with strings" << std::endl;
	testElementAccess(strBlob);
	testPointerAccess(strBlob);
	std::cout << std::endl;
	std::cout << "2) test with Integers" << std::endl;
	testElementAccess(intBlob);
	testPointerAccess(intBlob);

}
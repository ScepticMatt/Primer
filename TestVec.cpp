#include <string>
#include <sstream>
#include "Vec.h"
using StrVec = Vec<std::string>;
bool test1(), test2(), test3(), test4();
int main(){
	auto passed =  test1() && test2() && test3() && test4();
	std::cout << "tests " << (passed?"passed":"failed") << std::endl;
}
// test push_back and operator[]
bool test1(){
	StrVec a;
	a.push_back("Hello");
	a.push_back(" ");
	a.push_back("World");
	a.push_back("!");
	std::ostringstream ss;
	ss << a[0] << a[1] << a[2] << a[3];
	return ss.str() == "Hello World!";	
}
// test resize
bool test2(){
	StrVec b;
	b.resize(5);
	b[0]="H"; b[1]="e"; b[2]="l"; b[3]="l"; b[4]="o";
	std::ostringstream ss;
	ss << b[0] << b[1] << b[2] << b[3] << b[4];
	return ss.str() == "Hello";
}
// test reallocate
bool test3(){
	StrVec c;
	c.reserve(5);
	return c.capacity() >= 5;
}
// test initializer_list
bool test4(){
	StrVec d{"Hello", " ", "World", "!"};
	std::ostringstream ss;
	ss << d[0] << d[1] << d[2] << d[3];
	return ss.str() == "Hello World!";
}
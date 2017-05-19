#include <utility>
#include <type_traits>
// function that flips parameter 1 and 2 with perfect forwarding
template<typename F, typename T1, typename T2>
auto flip(F func, T1&& param1, T2&& param2) 
	-> typename std::result_of<F(T2&&,T1&&)>::type {  // can auto-deduct this line in c++14
	return func(std::forward<T2>(param2),std::forward<T1>(param1));
}
// test
#include <algorithm>
#include <iostream>
#include <functional>
std::string foo(std::string prefix, int suffix){
	return prefix + std::to_string(suffix);
}
int main(){
	// test with std::less
	std::cout << flip(std::less<int>(), 4, 5) << std::endl;
	// test with custom function
	std::cout << flip(foo, 5, "High ") << std::endl;

}
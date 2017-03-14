#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#define NDEBUG
void printVec (const std::vector<int>::iterator &begin,  const std::vector<int>::iterator &end){
	#ifndef NDEBUG
	std::clog << "In function: " << __func__ << ", "
              << "Vector size: " << end - begin << std::endl;
    #endif
	if (begin == end) { // empty vector
		std::cout << std::endl;
		return;
	}
	

	std:: cout << *begin << ((begin+1 == end) ? "" : " ");
	printVec(begin+1, end);
}
template <typename T> void readVec(std::vector<T> &vec){
	vec.erase(vec.begin(),vec.end());
	std::string line{};
	getline(std::cin, line);
	std::istringstream ss{line};
	T value;
	while (ss >> value) vec.push_back(value);
}
int main(){
	std::vector<int> vec;
	readVec(vec);
	printVec(vec.begin(), vec.end());
	return 0;
}
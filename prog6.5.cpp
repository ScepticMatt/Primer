#include <iostream>
#include <string>
#include <sstream> 
#include <stdexcept>

template <typename T> T abs(T value){
	//TODO: Handle overflows
	return (value > 0) ? value : -value;
	
}
template <typename T> T number{}; // C++14 variadic template
int main(){
	std::string line{};
	
	while(line == "")
		std::getline(std::cin, line);
	std::stringstream ss;

	char c{};
	if (ss.str(line) , ss >> number<long long> && !(ss >> c))	// input is integer
		std::cout << abs<long long>(number<long long>);
	else if (ss.str(line), ss >> number<long double>)			// input is float
		std::cout << abs<long double>(number<long double>);
	return 0;
}
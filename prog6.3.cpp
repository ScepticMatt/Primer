#include <iostream>
#include <stdexcept>

unsigned long long fact (unsigned value);
int main(){
	unsigned number;
	std::cout << "To calculate the factorial, enter a positive integer value" << std::endl;
	try{
		if (std::cin >> number){
			if (number < 0) 
				throw std::out_of_range("Can't calculate factorial of negative number");
			else if (number > 20)
				throw std::overflow_error("Too large, can't calculate");
			std::cout << "The factorial of " << number << " is " << fact(number) << std::endl;
			return 0;
		}
	} catch (std::out_of_range err){
		std::cerr << err.what() << std::endl;
		return -1;
	} catch (std::overflow_error err){
		std::cerr << err.what() << std::endl;
		return -1;
	}
}

unsigned long long fact (unsigned value){
	if (value < 2) return 1ull;
	long long result{1ull};
	while (value != 1)
		result *= value--;
	return result;
}


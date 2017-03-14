#include <vector>
#include <stdexcept>
#include <iostream>

int add (int a,int b){return a+b;}
int subtract (int a,int b){return a-b;}
int multiply (int a,int b){return a*b;}
int divide (int a,int b){
	try{
		if (!b) throw std::runtime_error("Divide by 0.");
		return a/b;
	} catch (std::runtime_error err){
		std::cerr << "Error in function " << __func__ << ": " << err.what() << std::endl;
		std::exit(-1);
	}

}
int main(){

	std::vector<int (*)(int,int)> vec;
	vec.push_back(add);
	vec.push_back(subtract);
	vec.push_back(multiply);
	vec.push_back(divide);
	int a,b;
	if (std::cin >> a >> b){
		for (auto func : vec)
			std::cout << func(a,b) << std::endl;

	}
	return 0;
}
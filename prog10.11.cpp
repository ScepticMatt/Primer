#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
template<typename T>
struct isShorter {
	isShorter(){};
	bool operator()(T a, T b) const{
		return a.size() < b.size();
	}
};
template<typename T, template <typename, typename> class Container, typename Comparator = std::less<T>>
void elimDups(Container<T, std::allocator<T>> &values, 
	void(*sorter)(typename Container<T, std::allocator<T>>::iterator, typename Container<T, std::allocator<T>>::iterator, Comparator) = std::sort,
	Comparator comp = Comparator()){
	sorter(values.begin(),values.end(),comp);
	auto end_unique = std::unique(values.begin(), values.end());
	values.erase(end_unique, values.end());
}


/*template<typename T, template <typename, typename> class Container>
bool isShorter(Container<T, std::allocator<T>> a, Container<T, std::allocator<T>> b){
	return a.size() < b.size();
}*/
int main(){
	// input
	std::vector<std::string> svec;
	for (std::string str; std::cin >> str; svec.emplace_back(str));
	// process
	elimDups(svec);
	elimDups(svec, std::stable_sort, isShorter<std::string>());//std::greater<std::string>()
	for (const auto& elem : svec) std::cout << elem << std::endl;
}
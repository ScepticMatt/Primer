#ifndef VEC_H
#define VEC_H
#include <memory>
#include <iostream>
#include <iterator>
#include <initializer_list>
#include <stdexcept>
template<typename> class Vec;
template<typename T> bool operator==(const Vec<T>&, const Vec<T>&);
template<typename T> bool operator!=(const Vec<T>&, const Vec<T>&);
template<typename T> bool operator<(const Vec<T>&, const Vec<T>&);
template<typename T> bool operator>(const Vec<T>&, const Vec<T>&);
using size_type = size_t;
template<typename T>
class Vec{
	friend bool operator==<T>(const Vec&, const Vec&);
	friend bool operator!=<T>(const Vec&, const Vec&);
	friend bool operator< <T>(const Vec&, const Vec&);
	friend bool operator> <T>(const Vec&, const Vec&);
public:
	// constructors
	Vec(): elements(nullptr), first_free(nullptr), cap(nullptr){};
	Vec(std::initializer_list<T> lst);
	//copy control:
	Vec(const Vec&);
	Vec(Vec&&) noexcept;
	Vec& operator=(const Vec&);
	Vec& operator=(Vec&&) noexcept;
	Vec& operator=(std::initializer_list<T>);
	~Vec();
	// member functions
	void push_back(const T&);
	void push_back(T&&);
	T pop_back();
	size_type size() const{ return first_free - elements;}
	size_type capacity() const{ return cap - elements;}
	void reserve(size_type);
	void resize(size_type);
	T& operator[](size_type pos){return elements[pos];}
	const T& operator[](size_type pos) const{return elements[pos];}
	T* begin() {return elements;}
	T* end() {return first_free;}
	const T* cbegin() const{return elements;}
	const T* cend() const{return first_free;}
private:
	// allocator:
	std::allocator<std::string> alloc;
	// utility functions:
	void chk_n_alloc(){ if(size() == capacity()) reallocate();} 
	std::pair<T*, T*> alloc_n_copy (const T*, const T*);
	void free();
	void reallocate();
	// members:
	T* elements;
	T* first_free;
	T* cap;
};
template<typename T>
Vec<T>::Vec(std::initializer_list<T> lst){
	auto newData = alloc_n_copy(lst.begin(), lst.end());
	elements = newData.first;
	first_free = cap = newData.second;
}
//. public member functions
// add member to the back of StrVec
template<typename T>
void Vec<T>::push_back(const T& data){
	chk_n_alloc();
	alloc.construct(first_free++, data);
}
template<typename T>
void Vec<T>::push_back(T&& data){
	chk_n_alloc();
	alloc.construct(first_free++, std::move(data));
}
// allocate extra storage
template<typename T>
T Vec<T>::pop_back(){
	if (elements == first_free) throw std::out_of_range("can't pop on empty vector");
	return *(first_free-- -1);
}
template<typename T>
void Vec<T>::reserve(size_type newCapacity){
	if (newCapacity > capacity()){
		// reallocate
		auto newElements = alloc.allocate(newCapacity);
		auto newFirst_free = newElements;
		// move data to new space
		for (auto it = elements; it != first_free; ++it)
			alloc.construct(newFirst_free++, std::move(*it));
		free();
		// refresh pointers
		elements = newElements;
		first_free = newFirst_free;
		cap = elements + newCapacity;
	}
}
// construct extra elements if needed
template<typename T>
void Vec<T>::resize(size_type newSize){
	if (newSize > size()){
		auto diff = newSize - size();
		for (unsigned i = 0; i != diff; ++i)
			push_back("");	// size increase
	}
			
}
/// copy control, allocate and construct exactly as many elements as vec has
template<typename T>
Vec<T>::Vec(const Vec& vec){
	auto newData = alloc_n_copy(vec.cbegin(), vec.cend());
	elements = newData.first;
	first_free = cap = newData.second;
}
template<typename T>
Vec<T>::Vec(Vec&& vec) noexcept: elements(vec.elements),
	first_free(vec.first_free),cap(vec.cap){
	vec.elements = vec.first_free = vec.cap = nullptr; // ready for delete
}
template<typename T>
Vec<T>& Vec<T>::operator=(const Vec& rhs){
	auto newData = alloc_n_copy(rhs.cbegin(), rhs.cend());
	elements = newData.first;
	first_free = cap = newData.second;
	return *this;
}
template<typename T>
Vec<T>::~Vec(){
	free();
}
template<typename T>
Vec<T>& Vec<T>::operator=(Vec&& rhs) noexcept{
	if (this != &rhs){
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}
template<typename T>
bool operator==(const Vec<T>& lhs, const Vec<T>& rhs){
	if (lhs.size() != rhs.size()) return false;
	for (auto it = lhs.elements, it2 = rhs.elements; it!=lhs.first_free; ++it, ++it2)
		if (*it != *it2) return false;	
	return true;
}
template<typename T>
bool operator!=(const Vec<T>& lhs, const Vec<T>& rhs){
	return !(lhs == rhs);
}
template<typename T>
bool operator<(const Vec<T>& lhs, const Vec<T>& rhs){
	auto it = lhs.elements, it2 = rhs.elements;
	for (;it!=lhs.first_free && it2!=rhs.first_free;++it,++it2)
		if (*it != *it2) return *it < *it2;
	if (it2 != rhs.first_free) return true; //lhs is suffix, this smaller
	return false;
}
template<typename T>
bool operator>(const Vec<T>& lhs, const Vec<T>& rhs){
	return !(lhs < rhs);
}
template<typename T>
Vec<T>& Vec<T>::operator=(std::initializer_list<T> rhs){
	auto data = alloc_n_copy(rhs.begin(),rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}
// returns begin and first past the last pointers
template<typename T>
std::pair<T*, T*> Vec<T>::alloc_n_copy
	(const T* begin, const T* end){
	auto newData = alloc.allocate(end-begin);
	return {newData, std::uninitialized_copy(begin, end, newData)};
}
// destroy and deallocate all string elements, in reverse order
template<typename T>
void Vec<T>::free(){
	if (elements){
		for (auto it = first_free; it != elements;)
			alloc.destroy(--it);
		alloc.deallocate(elements, capacity());
	}
}
// get more space and move all string elements
template<typename T>
void Vec<T>::reallocate(){
	// allocate new space
	auto newCapacity = capacity()?capacity()*2:1;
	auto newElements = alloc.allocate(newCapacity);
	auto newFirst_free = newElements;
	// move data to new space
	for (auto it = elements; it != first_free; ++it)
		alloc.construct(newFirst_free++, std::move(*it));
	free();
	elements = newElements;
	first_free = newFirst_free;
	cap = elements + newCapacity;	
}

#endif

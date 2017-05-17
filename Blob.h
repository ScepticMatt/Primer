#ifndef BLOB_H
#define BLOB_H
#include <vector>
#include <memory>
#include <initializer_list>
#include <string>
#include <stdexcept>
#include "BlobPtr.h"
#include "ConstBlobPtr.h"

// forward declarations of templates:
template<typename> class ConstBlobPtr;
template<typename> class BlobPtr;
template<typename> class Blob;
template<typename T> bool operator==(const Blob<T>&, const Blob<T>&);
template<typename T> bool operator!=(const Blob<T>&, const Blob<T>&);
template<typename T> bool operator<(const Blob<T>&, const Blob<T>&);
template<typename T> bool operator>(const Blob<T>&, const Blob<T>&);
template<typename T> bool operator<=(const Blob<T>&, const Blob<T>&);
template<typename T> bool operator>=(const Blob<T>&, const Blob<T>&);

template<typename T> class Blob{ 
	friend class BlobPtr<T>;
  	friend class ConstBlobPtr<T>;	
	friend bool operator==<T>(const Blob<T>&, const Blob<T>&);
	friend bool operator!=<T>(const Blob<T>&, const Blob<T>&);
	friend bool operator< <T>(const Blob<T>&, const Blob<T>&);	// mind the space
	friend bool operator> <T>(const Blob<T>&, const Blob<T>&);
	friend bool operator<=<T>(const Blob<T>&, const Blob<T>&);
	friend bool operator>=<T>(const Blob<T>&, const Blob<T>&);
public:
	using vec = typename std::vector<T>;
	using sz = typename vec::size_type;
	using iterator = BlobPtr<T>;
	using const_iterator = ConstBlobPtr<T>;
	Blob(): data(std::make_shared<vec>()){}
	Blob(std::initializer_list<T> il): data(std::make_shared<vec>(il)){}
// copy control:
	Blob(const Blob& obj): data(std::make_shared<vec>(*obj.data)){}
	Blob(Blob&& obj): data(std::make_shared<vec>(std::move(*obj.data))){}
	Blob& operator=(const Blob& rhs){
		data = std::make_shared<vec>(*rhs.data);
		return *this;
	}
	Blob& operator=(Blob&& rhs){
		data = std::make_shared<vec>(std::move(*rhs.data));
		return *this;
	}
// information functions
	sz size() const {return data->size();}
	bool empty() const { return data->empty();}
// element access
	void push_back(const T& value){data->push_back(value);}
	void push_back(T&& value){data->push_back(std::move(value));}
	T& pop_back(){return data->pop_back();}

	// get element without range check
	T& operator[](const sz& pos){return (*data)[pos];}
	const T& operator[](const sz& pos) const {return (*data)[pos];}
	// get element with range check
	T& at(const sz& pos){
		check(pos, "out of range");
		return (*data)[pos];
	}
	const T& at(const sz& pos) const{
		check(pos, "out of range");
		return (*data)[pos];
	}
	T& front(){
		check(0, "Blob is empty");
		return data->front();
	}
	const T& front() const{
		check(0, "Blob is empty");
		return data->front();
	}
	T& back(){
		check(0, "Blob is empty");
		return data->back();
	}
	const T& back() const{
		check(0, "Blob is empty");
		return data->back();
	}
	// return iterators:
	iterator begin(){ return iterator(*this);}
	iterator end(){ return iterator(*this, size());}
	const_iterator cbegin() const{ return const_iterator(*this);}
	const_iterator cend() const{ return const_iterator(*this, size());}
private:
	void check(sz i, const std::string& msg) const{	// utility range checker
		if (i >= data->size()) 
			throw std::out_of_range(msg);
	}	
	std::shared_ptr<vec> data;						// private data member
};
// comparison operator definitions:
template<typename T> 
bool operator==(const Blob<T>& lhs, const Blob<T>& rhs){ return *lhs.data == *rhs.data;}
template<typename T>
bool operator!=(const Blob<T>& lhs, const Blob<T>& rhs){ return *lhs.data != *rhs.data;}
template<typename T>
bool operator<(const Blob<T>& lhs, const Blob<T>& rhs){ return *lhs.data < *rhs.data;}
template<typename T>
bool operator>(const Blob<T>& lhs, const Blob<T>& rhs){ return *lhs.data > *rhs.data;}
template<typename T>
bool operator<=(const Blob<T>& lhs, const Blob<T>& rhs){ return *lhs.data <= *rhs.data;}
template<typename T>
bool operator>=(const Blob<T>& lhs, const Blob<T>& rhs){ return *lhs.data >= *rhs.data;}
#endif
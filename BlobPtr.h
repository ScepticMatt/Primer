#ifndef BLOBPTR_H
#define BLOBPTR_H
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include "Blob.h"

// forward declarations of templates:
template<typename> class BlobPtr;
template<typename> class Blob;
template<typename T> bool operator==(const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator!=(const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator<(const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator>(const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator<=(const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator>=(const BlobPtr<T>&, const BlobPtr<T>&);

template<typename T>
class BlobPtr{
	friend bool operator==<T>(const BlobPtr<T>&, const BlobPtr<T>&);
	friend bool operator!=<T>(const BlobPtr<T>&, const BlobPtr<T>&);
	friend bool operator< <T>(const BlobPtr<T>&, const BlobPtr<T>&);	// mind the space
	friend bool operator> <T>(const BlobPtr<T>&, const BlobPtr<T>&);
	friend bool operator<=<T>(const BlobPtr<T>&, const BlobPtr<T>&);
	friend bool operator>=<T>(const BlobPtr<T>&, const BlobPtr<T>&);
public:
	using vec = typename std::vector<T>;
	using sz = typename vec::size_type;
	using diff = typename vec::difference_type;
	BlobPtr(): wptr(nullptr), curr(0){}
	explicit BlobPtr(Blob<T>& obj, sz pos = 0): wptr(obj.data), curr(pos){ check(pos, "out of range");}
	// member operators:
	T& operator*(){
		return (*check(curr+1,"cannot dereference past the end iterator"))[curr];
	}
	T& operator->(){
		return &**this;			// &operator*(*this)
	}
	BlobPtr& operator++()&{		// prefix increment, cannot be used on temporary
		check(curr+1,"increment beyond past the end");
		curr++;
		return *this;
	}
	BlobPtr operator++(int)&{	// postfix increment, cannot be used on temporary
		BlobPtr ret = *this;
		++*this;				// operator++()
		return ret;
	}
	BlobPtr& operator--()&{		// prefix decrement, cannot be used on temporary
		check(curr-1,"canot dereferene before the begin iterator");
		curr--;
		return *this;
	}
	BlobPtr operator--(int)&{	// postfix decrement, cannot be used on temporary
		BlobPtr ret = *this;
		--*this;				// operator--()
		return ret;
	}
	BlobPtr operator+(diff offset) const{
		check(curr+offset,"increased beyond past the end");
		BlobPtr ret = *this;
		ret.curr += offset;
		return ret;
	}
	BlobPtr operator-(diff offset) const{
		check(curr-offset,"decreased before the begin");
		BlobPtr ret = *this;
		ret.curr -= offset;
		return ret;
	}
	BlobPtr& operator+=(diff offset)&{	// compound assignement, cannot be used on temporary
		check(curr+offset,"increased beyond past the end");
		curr+=offset;
		return *this;
	}
	BlobPtr& operator-=(diff offset)&{	// compound assignement, cannot be used on temporary
		check(curr-offset,"decreased before the begin");
		curr-=offset;
		return *this;		
	}
private:
	std::shared_ptr<vec> check(sz pos, const std::string& msg) const{
		auto sptr = wptr.lock();
		if (!sptr) 
			throw std::runtime_error("can't lock BlobPtr");
		if (pos > sptr->size())	// past the end iterator is ok.
	  		throw std::out_of_range(msg);
		return sptr;
	}
	std::weak_ptr<vec> wptr;
	sz curr;
};

template<typename T>	// lhs and rhs point to the same element
bool operator==(const BlobPtr<T>& lhs, const BlobPtr<T>& rhs){ 
	return lhs.wptr.lock() == rhs.wptr.lock() && lhs.curr == rhs.curr;
}
template<typename T>
bool operator!=(const BlobPtr<T>& lhs, const BlobPtr<T>& rhs){ 
	return !(lhs == rhs);
}
template<typename T>	// lhs points to an element before rhs
bool operator<(const BlobPtr<T>& lhs, const BlobPtr<T>& rhs){ 
	return lhs.wptr.lock() == rhs.wptr.lock() && lhs.curr < rhs.curr;
}
template<typename T>	// lhs points to an element before rhs
bool operator>(const BlobPtr<T>& lhs, const BlobPtr<T>& rhs){ 
	return lhs.wptr.lock() == rhs.wptr.lock() && lhs.curr > rhs.curr;
}
template<typename T>	// lhs points to an element before rhs
bool operator<=(const BlobPtr<T>& lhs, const BlobPtr<T>& rhs){ 
	return !(lhs > rhs);
}
template<typename T>	// lhs points to an element before rhs
bool operator>=(const BlobPtr<T>& lhs, const BlobPtr<T>& rhs){ 
	return !(lhs < rhs);
}
#endif
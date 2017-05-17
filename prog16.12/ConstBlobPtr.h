#ifndef CONSTBLOBPTR_H
#define CONSTBLOBPTR_H
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include "Blob.h"

// forward declarations of templates:
template<typename> class ConstBlobPtr;
template<typename> class Blob;
template<typename T> bool operator==(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator!=(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator<(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator>(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator<=(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator>=(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);

template<typename T>
class ConstBlobPtr{
	friend bool operator==<T>(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
	friend bool operator!=<T>(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
	friend bool operator< <T>(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);	// mind the space
	friend bool operator> <T>(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
	friend bool operator<=<T>(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
	friend bool operator>=<T>(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
public:
	using vec = typename std::vector<T>;
	using sz = typename vec::size_type;
	using diff = typename vec::difference_type;
	ConstBlobPtr(): wptr(nullptr), curr(0){}
	explicit ConstBlobPtr(const Blob<T>& obj, sz pos = 0): wptr(obj.data), curr(pos){ check(pos, "out of range");}
	// member operators:
	const T& operator*(){
		return (*check(curr+1,"cannot dereference past the end iterator"))[curr];
	}
	const T& operator->(){
		return &**this;			// &operator*(*this)
	}
	ConstBlobPtr& operator++()&{		// prefix increment, cannot be used on temporary
		check(curr+1,"increment beyond past the end");
		curr++;
		return *this;
	}
	ConstBlobPtr operator++(int)&{	// postfix increment, cannot be used on temporary
		ConstBlobPtr ret = *this;
		++*this;				// operator++()
		return ret;
	}
	ConstBlobPtr& operator--()&{		// prefix decrement, cannot be used on temporary
		check(curr-1,"canot dereferene before the begin iterator");
		curr--;
		return *this;
	}
	ConstBlobPtr operator--(int)&{	// postfix decrement, cannot be used on temporary
		ConstBlobPtr ret = *this;
		--*this;				// operator--()
		return ret;
	}
	ConstBlobPtr operator+(diff offset) const{
		check(curr+offset,"increased beyond past the end");
		ConstBlobPtr ret = *this;
		ret.curr += offset;
		return ret;
	}
	ConstBlobPtr operator-(diff offset) const{
		check(curr-offset,"decreased before the begin");
		ConstBlobPtr ret = *this;
		ret.curr -= offset;
		return ret;
	}
	ConstBlobPtr& operator+=(diff offset)&{	// compound assignement, cannot be used on temporary
		check(curr+offset,"increased beyond past the end");
		curr+=offset;
		return *this;
	}
	ConstBlobPtr& operator-=(diff offset)&{	// compound assignement, cannot be used on temporary
		check(curr-offset,"decreased before the begin");
		curr-=offset;
		return *this;		
	}
private:
	std::shared_ptr<vec> check(sz pos, const std::string& msg) const{
		auto sptr = wptr.lock();
		if (!sptr) 
			throw std::runtime_error("can't lock ConstBlobPtr");
		if (pos > sptr->size())	// past the end iterator is ok.
	  		throw std::out_of_range(msg);
		return sptr;
	}
	std::weak_ptr<vec> wptr;
	sz curr;
};

template<typename T>	// lhs and rhs point to the same element
bool operator==(const ConstBlobPtr<T>& lhs, const ConstBlobPtr<T>& rhs){ 
	return lhs.wptr.lock() == rhs.wptr.lock() && lhs.curr == rhs.curr;
}
template<typename T>
bool operator!=(const ConstBlobPtr<T>& lhs, const ConstBlobPtr<T>& rhs){ 
	return !(lhs == rhs);
}
template<typename T>	// lhs points to an element before rhs
bool operator<(const ConstBlobPtr<T>& lhs, const ConstBlobPtr<T>& rhs){ 
	return lhs.wptr.lock() == rhs.wptr.lock() && lhs.curr < rhs.curr;
}
template<typename T>	// lhs points to an element before rhs
bool operator>(const ConstBlobPtr<T>& lhs, const ConstBlobPtr<T>& rhs){ 
	return lhs.wptr.lock() == rhs.wptr.lock() && lhs.curr > rhs.curr;
}
template<typename T>	// lhs points to an element before rhs
bool operator<=(const ConstBlobPtr<T>& lhs, const ConstBlobPtr<T>& rhs){ 
	return !(lhs > rhs);
}
template<typename T>	// lhs points to an element before rhs
bool operator>=(const ConstBlobPtr<T>& lhs, const ConstBlobPtr<T>& rhs){ 
	return !(lhs < rhs);
}
#endif

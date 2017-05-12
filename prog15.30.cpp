// (c) Matthias Hutter, 2017

#include <string>		// std::string
#include <iostream>		// cout, clog, ostream
#include <memory>		// shared_ptr
#include <vector>
#include <set>			// multiset
#include <algorithm>	// upper_bound
#define DEBUG 0			// set to 1 toenable logging
class Quote{
public:
	// constructors
	Quote() = default;	// default constructor
	Quote(const std::string& book, double sales_price):
		bookNo(book), price(sales_price){}
	// copy control, rule of five
	Quote(const Quote&);
	Quote(Quote&&);
	Quote& operator=(const Quote&);
	Quote& operator=(Quote&&);
	virtual ~Quote();
	std::string isbn() const { return bookNo;}
	// derived classes will override and apply different versions
	virtual Quote* clone() const&{ return new Quote(*this);}
	virtual Quote* clone() &&{ return new Quote(std::move(*this)); }
	virtual double net_price(std::size_t n) const{ return n*price;}
	virtual std::ostream& debug(std::ostream&) const;
	
private:
	std::string bookNo; // derived classes need to use the isbn() member
protected:
	double price = 0.0;	// undiscounted price;
};
// copy control;
Quote::Quote(const Quote& obj): bookNo(obj.bookNo), price(obj.price){
	std::clog << "called Quote::Quote(const Quote&)" << std::endl;
}
Quote::Quote(Quote&& obj): bookNo(std::move(obj.bookNo)), price(std::move(obj.price)){
	std::clog << "called Quote::Quote(Quote&&)" << std::endl;
}
Quote& Quote::operator=(const Quote& rhs){
	bookNo = rhs.bookNo;
	price = rhs.price;
	std::clog << "called Quote& Quote::operator=(const Quote&)" << std::endl;
	return *this;
}
Quote& Quote::operator=(Quote&& rhs){
	bookNo = std::move(rhs.bookNo);
	price = std::move(rhs.price);
	std::clog << "called Quote& Quote::operator=(Quote&&)" << std::endl;	
}
Quote::~Quote(){
	std::clog << "called Quote::~Quote()" << std::endl;
}
std::ostream& Quote::debug(std::ostream& os) const{
	os 	<< "data members:"
		<< " Quote::bookNo " << bookNo
		<< " Quote::price " << price;
}
// Abstract discount class
class Disc_quote: public Quote{
public:
	Disc_quote() = default;
	Disc_quote(const std::string& book, double sales_price,
		std::size_t quanity_, double discount_):
		Quote(book, sales_price), quantity(quanity_), discount(discount_){}
	// copy control
	Disc_quote(const Disc_quote&);
	Disc_quote(Disc_quote&&);
	Disc_quote& operator=(const Disc_quote&);
	Disc_quote& operator=(Disc_quote&&);
	~Disc_quote() override;
	double net_price(std::size_t n) const override = 0; // pure virtual
	std::ostream& debug(std::ostream&) const override = 0;	// pure virtual
protected:
	size_t quantity = 0;
	double discount = 0;
};
// copy control:
Disc_quote::Disc_quote(const Disc_quote& obj): Quote::Quote(obj), 
	quantity(obj.quantity), discount(obj.discount){
	std::clog << "called Disc_quote::Disc_quote(const Disc_quote&)" << std::endl;
}
Disc_quote::Disc_quote(Disc_quote&& obj): Quote::Quote(obj),
	quantity(std::move(obj.quantity)), discount(std::move(obj.discount)){
	std::clog << "called Disc_quote::Disc_quote(Disc_quote&&)" << std::endl;
}
Disc_quote& Disc_quote::operator=(const Disc_quote& rhs){
	Quote::operator=(rhs);
	quantity = rhs.quantity;
	discount = rhs.discount;
	std::clog << "called Disc_quote& Disc_quote::operator=(const Disc_quote&)" << std::endl;
	return *this;
}
Disc_quote& Disc_quote::operator=(Disc_quote&& rhs){
	Quote::operator=(std::move(rhs));
	quantity = std::move(rhs.quantity);
	discount = std::move(rhs.quantity);
	std::clog << "called Disc_quote& Disc_quote::operator=(Disc_quote&&)" << std::endl;
	return *this;
}
Disc_quote::~Disc_quote(){
	std::clog << "called Disc_quote::~Disc_quote()" << std::endl;
}
class Bulk_quote: public Disc_quote{
public:
	Bulk_quote() = default;
	using Disc_quote::Disc_quote;
	// copy control
	Bulk_quote(const Bulk_quote&);
	Bulk_quote(Bulk_quote&&);
	Bulk_quote& operator=(const Bulk_quote&);
	Bulk_quote& operator=(Bulk_quote&&);
	~Bulk_quote() override;	// also virtual
	// virtual override
	Bulk_quote* clone() const & override{ return new Bulk_quote(*this);}
	Bulk_quote* clone() && override{ return new Bulk_quote(std::move(*this));}
	double net_price(std::size_t n) const override;
	std::ostream& debug(std::ostream&) const override;
	
};
// copy control
Bulk_quote::Bulk_quote(const Bulk_quote& obj): Disc_quote(obj){
	std::clog << "called Bulk_quote::Bulk_quote(const Bulk_quote&)" << std::endl;
}
Bulk_quote::Bulk_quote(Bulk_quote&& obj): Disc_quote(std::move(obj)){
	std::clog << "called Bulk_quote::Bulk_quote(Bulk_quote&&)" << std::endl;
}
Bulk_quote& Bulk_quote::operator=(const Bulk_quote& obj){
	Disc_quote::operator=(obj);
	std::clog << "called Bulk_quote::operator=(const Bulk_quote&)" << std::endl;
	return *this;
}
Bulk_quote& Bulk_quote::operator=(Bulk_quote&& obj){
	Disc_quote::operator=(std::move(obj));
	std::clog << "called Bulk_quote::operator=(Bulk_quote&&)" << std::endl;
	return *this;
}
Bulk_quote::~Bulk_quote(){
	std::clog << "called Bulk_quote::~Bulk_quote()" << std::endl;
}
std::ostream& Bulk_quote::debug(std::ostream& os) const{
	os 	<< "data members: "
		<< " Bulk_quote::min_quantity " << quantity
		<< " Bulk_quote::discount " << discount;
}
double Bulk_quote::net_price(std::size_t n) const{
	if (n > quantity)
		return n*price*(1-discount);
	else return n*price;
}
class Limited_quote: public Disc_quote{
public:
	Limited_quote() = default;
	using Disc_quote::Disc_quote;
	// uses sythesized copy control here
	// virtual override
	Limited_quote* clone() const & override{ return new Limited_quote(*this);}
	Limited_quote* clone() && override{ return new Limited_quote(std::move(*this));}
	std::ostream& debug(std::ostream&) const override;
	double net_price(std::size_t n) const override;
};
std::ostream& Limited_quote::debug(std::ostream& os) const{
	os 	<< "data members: "
		<< " Limited_quote::max_quantity " << quantity
		<< " Limited_quote::discount " << discount;
}
double Limited_quote::net_price(std::size_t n) const{
	if (n > quantity) 
		return price*(n - quantity*discount);
	else return price*n*(1-discount);
}
double print_total(std::ostream& os, const Quote& item, size_t n){
	double total = item.net_price(n);
	os 	<< "ISBN: " << item.isbn() 
		<< " amount sold: " << n 
		<< " total due: " << total << std::endl;
	return total;
}
class Basket{
public:
	void add_item(const Quote& sale){
		items.insert(std::shared_ptr<Quote>(sale.clone()));
	}
	void add_item(Quote&& sale){
		items.insert(std::shared_ptr<Quote>(std::move(sale).clone()));
	}
	double total_receipt(std::ostream&) const;
private:
	static bool compare(const std::shared_ptr<Quote>& lhs, const std::shared_ptr<Quote>& rhs){
		return lhs->isbn() < rhs->isbn();
	}
	// multiset to hold Quotes, using custom Comparator compare
	std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{compare};
};
double Basket::total_receipt(std::ostream& os) const{
	double sum = 0.; //total due
	for (auto it = items.cbegin(); it != items.cend(); it = items.upper_bound(*it))
		sum += print_total(os, **it, items.count(*it));	
	os << "All combined due: " << sum << std::endl;
	return sum;
}
// tests print_total
void test1(){
	Quote a{"0-201-82470-1",10};
	Bulk_quote b{"0-201-82470-1",10,20,0.1};
	Bulk_quote c{b};
	print_total(std::cout, a,10);
	print_total(std::cout, b,10);
	print_total(std::cout, c,50);
}
// tests instantiation of abstract class Disc_quote
void test2(){
	//Disc_quote d; // error: cannot declare variable 'd' to be of abstract type 'Disc_quote'
}
// tests vector filled with Quote or inherited class types
void test3(){
	std::vector<std::shared_ptr<Quote>> sales;
	sales.push_back(std::make_shared<Quote>("0-201-82470-1", 100));
	sales.push_back(std::make_shared<Bulk_quote>("0-201-54848-8", 50, 10, .25));
	sales.push_back(std::make_shared<Limited_quote>("0-201-54844-6", 30, 20, .1));
	double sum = 0.;
	for (const auto& elem: sales)
		sum += elem->net_price(60);
	std::cout << "Net sales: " << sum << std::endl;
}
// tests copy control
void test4(){
	std::cout << "---------------------------------" << std::endl;
	Quote qempty;
	Quote qnormal("0-201-82470-1", 100);
	Quote qcopy(qnormal);
	qempty.debug(std::cout) << std::endl;
	qnormal.debug(std::cout) << std::endl;
	qcopy.debug(std::cout) << std::endl;
	Quote qmove(std::move(qcopy));
	qmove.debug(std::cout) << std::endl;
	qcopy.debug(std::cout) << std::endl;	// undefined after move
	std::cout << "---------------------------------" << std::endl;
	Bulk_quote bempty;
	Bulk_quote bnormal("0-201-54848-8", 50, 10, .25);
	Bulk_quote bcopy(bnormal);
	bempty.debug(std::cout) << std::endl;
	bnormal.debug(std::cout) << std::endl;
	bcopy.debug(std::cout) << std::endl;
	Bulk_quote bmove(std::move(bcopy));
	bmove.debug(std::cout) << std::endl;
	bcopy.debug(std::cout) << std::endl;	// undefined after move
	std::cout << "---------------------------------" << std::endl;
	Limited_quote lempty;
	Limited_quote lnormal("0-201-54844-6", 30, 20, .1);
	Limited_quote lcopy(lnormal);
	lempty.debug(std::cout) << std::endl;
	lnormal.debug(std::cout) << std::endl;
	lcopy.debug(std::cout) << std::endl;
	Limited_quote lmove(std::move(lcopy));
	lmove.debug(std::cout) << std::endl;
	lcopy.debug(std::cout) << std::endl;	// undefined after move
}
// tests Basket class
test5(){
	Basket basket;
	Quote qnormal("0-201-82470-1", 100);
	Bulk_quote bnormal("0-201-54848-8", 50, 10, .25);
	Limited_quote lnormal("0-201-54844-6", 30, 20, .1);
	for (auto i = 0; i != 10; ++i){
		basket.add_item(qnormal);
		basket.add_item(bnormal);
		basket.add_item(lnormal);
	}
	basket.total_receipt(std::cout);
}

int main(){
#if DEBUG == 0
	std::clog.setstate(std::ios_base::failbit);	// disable logging
#endif 
	test1();
	test2();	
	test3();
	test4();
	test5();
}

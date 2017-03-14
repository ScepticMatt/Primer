#include <string>
struct Sales_data {
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};
bool read(Sales_data& target);
Sales_data sum(const Sales_data& item1, const Sales_data& item2);
Sales_data sum(Sales_data&& item1, Sales_data&& item2);
void print(const Sales_data& item);
void print(const Sales_data&& item);

bool read(Sales_data& target){
	double price;
	if(!(std:: cin >> target.bookNo >> target.units_sold >> price))
		return false;
	target.revenue = target.units_sold * price;
	return true;
}
void print(const Sales_data& item){
	std::cout << item.bookNo << " " << item.units_sold << " " << item.revenue << " " 
			  << item.revenue/item.units_sold // average price
			  << std::endl;
}
void print(const Sales_data&& item){print(item);}

Sales_data sum(const Sales_data& item1,const Sales_data& item2){
	Sales_data newItem;
	if (item1.bookNo == item2.bookNo){
		newItem.bookNo = item1.bookNo;
		newItem.units_sold = item1.units_sold + item2.units_sold;
		newItem.revenue = item1.revenue + item2.revenue;
	}
	else{
		std::cerr << "Error: Different ISBN, exiting" << std::endl;
		std::exit(1);
	}
	
	return newItem;
}
Sales_data sum(Sales_data&& item1, Sales_data&& item2){
	return sum(item1, item2);
}

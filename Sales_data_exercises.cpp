#include <iostream>
#include <algorithm>
#include <vector>

#include "Sales_data.h"

void task20(), task21(), task22(), task23();
int main(){
	// test functions
	//task20();
	//task21();
	//task22();
	task23();	

}	
// 1.20: reads a set of book sales, writing each transaction to the standard output
void task20(){
	Sales_data currentSale;
	if (read(currentSale))
		print(currentSale);
}
// 1.21: reads two Sales_data objects that have the same ISBN and produce their sum
void task21(){
	Sales_data data1, data2;
	if (read(data1) && read(data2))
		print(sum(data1, data2));	
}
// 1.22: Reads several transactions for the same ISBN and prints the sum of the result
void task22(){
	Sales_data totalSale, currentSale;
	read(totalSale);	// first transaction
	while(read(currentSale) && totalSale.bookNo == currentSale.bookNo){
		totalSale = sum(totalSale,currentSale);
	}
	std::cout << "End of current transaction" << std::endl;
	print(totalSale);
}

//Reads several transactions, counts and outputs how many sales for each ISBN
void task23(){
	// input Sales_data transactions
	std::vector<Sales_data> transactions;
	Sales_data currentSale;
	while(read(currentSale))
		transactions.push_back(currentSale);

	// Counts how many transactions for each ISBN 
	for (auto it = transactions.begin(), it_end = transactions.end(); it < it_end; ++it){	// for each ISBN
		unsigned currentAmount = it->units_sold;
		it_end = remove_if(it+1, it_end, 			// shifts all elements with the same ISBN to end
			[&it, &currentAmount](Sales_data test){	// lambda: check ISBN, count sales 
				return (test.bookNo == it->bookNo) ? currentAmount += test.units_sold, true : false;
			});	
		std::cout << it->bookNo << " "<< currentAmount << std::endl;	// output sum for ISBN
	}
}
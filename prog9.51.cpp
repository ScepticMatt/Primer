#include <string>
#include <cstring>	//for strlen
#include <iostream>
#include <vector>
#include <array>
#include <regex>
#include <chrono>
#include <ctime>
#include <algorithm>
bool test();

class USDate{
private:
	//date formats:
	static const std::regex shortMDY;		// 4/1/16 or 04/01/2016
	static const std::regex longMDY;		// Apr 1(st), 2016 or April 1(st), 2016 
	static const std::regex longDMY;		// 1(st) of April, 2016	
	static const std::regex militaryDMY;	// 1 APR 2016
	static const std::regex iso8601;		// 2016-04-01
	//month words
	static constexpr std::array<char[4],12> shortMonths{	// std::string can't be constexpr
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", 
		"Aug", "Sep", "Oct", "Nov", "Dec"};
	static constexpr std::array<char[10],12> longMonths{
		"January", "February", "March", "April", "May", "June", 
		"July", "August", "September", "October", "November", "December"};
	std::string outputISODate() const{return std::string(getYear()) + "-" + getMonth() + "-" + getDay();}
	unsigned year, month, day;
	static unsigned currentYear() {
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);
		tm* timePtr = localtime(&in_time_t);
		return timePtr->tm_year + 1900;
	}
	// converts yy to yyyy. Example: 96 -> 1996 , 17 -> 2017 (in the year 2017)
	static make_long(unsigned& year){
		auto current = currentYear();
		year += 100*((year <= current%100)? current/100 : current/100 - 1);	// this century or previous
	}
	void setMonth(const std::string& month_str){
		for (unsigned i = 0; i != 13; ++i){
			if (month_str == shortMonths[i] || month_str == longMonths[i] ){
				month = i+1;
				return;
			}
		}
		throw std::invalid_argument("No match in month string");
	}
public:
	friend std::ostream &print(std::ostream &os, const USDate &d);
	USDate(const std::string& input) {
		std::smatch match;
		if (std::regex_search(input,match,shortMDY)) {
			month = std::stoi(match[1]);
			day = std::stoi(match[2]);
			year = std::stoi(match[3]);
			if (year < 100) make_long(year);
		}
		else if (std::regex_search(input,match,longMDY)){
			setMonth(match[1]);
			day = std::stoi(match[2]);
			year = std::stoi(match[3]);
		}
		else if (std::regex_search(input,match,longDMY)){
			day = std::stoi(match[1]);
			setMonth(match[2]);
			year = std::stoi(match[3]);
		}
		else if (std::regex_search(input,match,militaryDMY)){
			day = std::stoi(match[1]);	
			std::string month_str = match[2];
			std::transform(month_str.begin()+1, month_str.end(), month_str.begin()+1, ::tolower); //convert APR to Apr
			setMonth(month_str);
			year = std::stoi(match[3]);
		}
		else if (std::regex_search(input,match,iso8601)){
			year = std::stoi(match[1]);
			month = std::stoi(match[2]);
			day = std::stoi(match[3]);
		}
		else throw std::invalid_argument("No matching date format found");
	}
	bool operator==(USDate date_){
		return date_.year==year && date_.month==month && date_.day==day;
	}
	std::string getDay() const{return std::to_string(day);}
	std::string getMonth() const{return std::to_string(month);}
	std::string getYear() const{return std::to_string(year);}


};
// Have to define outside (Until C++17, when inline static members with initializer are defined)
constexpr std::array<char[4],12> USDate::shortMonths;
constexpr std::array<char[10],12> USDate::longMonths;
//formats. regex compile done at runtime
const std::regex USDate::shortMDY{R"(^(\d{1,2})[./](\d{1,2})[./](\d{2}|\d{4})$)"};				// 4/1/16 or 04/01/2016
const std::regex USDate::longMDY{R"(^([A-Z][a-z]{2,8}) (\d{1,2})(?:st|rd|th)?, (\d{4})$)"};		// Apr 1(st), 2016 or April 1(st), 2016 
const std::regex USDate::longDMY{R"(^(\d{1,2})(?:st|rd|th)? of ([A-Z][a-z]{2,8}), (\d{4})$)"};	// 1(st) of April, 2016	
const std::regex USDate::militaryDMY{R"(^(\d{2}) ([A-Z]{3}) (\d{4})$)"};						// 1 APR 2016
const std::regex USDate::iso8601{R"(^(\d{4})-(\d{2})-(\d{2})$)"};								// 2016-04-01
std::ostream &print(std::ostream &os, const USDate& d){os << d.outputISODate();}

int main(){
	if (test()){
		std::cout << "test succedded";
		return 0;
	}
	else {
		std::cout << "test failed";
		return -1;
	}
}

// returns true if tests succeed
bool test(){
	const std::vector<std::string> testDates{
		"Apr 01, 2016", "April 1, 2016", "Apr 1st, 2016", "4/1/16",
		"4/1/2016",	"04/01/16", "04/01/2016", "2016-04-01",
		"01 APR 2016", "1st of April, 2016"};
	for (auto date : testDates){
		USDate test(date);
		if (test.getYear() != "2016" || test.getMonth() != "4" || test.getDay() != "1") {
			std::cerr << "Missmach in " << date << ": ";
			std::cerr << test.getYear() << " " << test.getMonth() << " "<< test.getDay() << std::endl;
			return false;
		}
	}
	return true;
}

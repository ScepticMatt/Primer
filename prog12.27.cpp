#include <vector>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

using std::vector;
using std::string;
using std::map;
using std::set;
using std::shared_ptr;
using line_no = vector<string>::size_type;

class QueryResult;
class TextQuery{
public:
	TextQuery(std::istream& in);
	// perform query
	QueryResult query(const string& word) const;
private:
	shared_ptr<vector<string>> lines;
	map<string, shared_ptr<set<line_no>>> wordMap;
};

class QueryResult{
public:
	// fetch line numbers associated with a given word
	QueryResult(string queryWord_, shared_ptr<set<line_no>> lineNumbers_, 
		shared_ptr<vector<string>> lines_)
		:queryWord(queryWord_), lineNumbers(lineNumbers_), lines(lines_) {};
	// print the text appearing in the input file at a given line number
	friend std::ostream& print(std::ostream&, const QueryResult&);
private:
	string queryWord;
	shared_ptr<set<line_no>> lineNumbers;	// numbers of the lines where queryWord was found
	shared_ptr<vector<string>> lines;		// pointer to line storage
};
TextQuery::TextQuery(std::istream& in) : lines(new vector<string>){
	// remember lines
	string line;
	while (getline(in, line)){					// read line by line
		lines->push_back(line);					// save line
		line_no pos = lines->size()-1;
		// break input into separate words
		std::istringstream ss(line);
		string word;
		while(ss >> word){
			auto& lines = wordMap[word];		// a set in WordMap
			if (!lines)
				lines.reset(new set<line_no>);	// allocate new set, reuse control block
			lines->insert(pos);					// insert line number
		}
	}
}
QueryResult TextQuery::query(const string& word) const{
	static shared_ptr<set<line_no>> nodata(new set<line_no>);	// empty set
	auto search = wordMap.find(word);
	if (search == wordMap.end()) 
		return QueryResult(word, nodata, lines);				// not found
	else 
		return QueryResult(word, search->second, lines);		// found
}

std::ostream& print(std::ostream& os, const QueryResult& result){
	os << result.queryWord << " occurs " << result.lineNumbers->size() << " time"
		<< ((result.lines->size() == 1) ? "": "s") << std::endl;
	for (auto num : *result.lineNumbers)
		os << "\t(line " << num + 1 << ") " 
			<< *(result.lines->begin() + num) << std::endl;
	return os;
}
void runQueries(std::ifstream& infile){
	TextQuery tq(infile);	// store the file and build the query map
	// iterate with the user: prompt for a word to find and print results
	while(true){
		std::cout << "Enter word to look for, or q to quit: ";
		string s;
		// stop if we hit end-of-file on the input or if a 'q' is entered
		if (!(std::cin >> s) || s == "q") break;
		print(std::cout, tq.query(s)) << std::endl;
	}
}
int main(int argc, char** argv){
	if (argc < 2){
		std::cerr << "Error: no input file" << std::endl
			<< "Usage: " << argv[0] << " <filename>" << std::endl;
		exit(-1);
	} 
		std::ifstream in(argv[1]);
	if (in)
		runQueries(in);
	return 0;
}

#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
using sz = std::vector<std::string>::size_type;

// Classless solution to the word query problem.
// generates map of each word for a given input file
void generate_map(std::vector<std::string>& lines, 
	std::map<std::string, std::set<sz>>& wordMap, 
	std::istream& in){
	// read lines
	std::string line;
	while(getline(in, line)){
		lines.push_back(line);
		// break line into words
		std::istringstream ss(line);
		std::string word;
		sz position = lines.size() - 1;
		while(ss >> word){
			wordMap[word].insert(position);	// save position
		}
	}

}
// lookup word, output results
void query(std::vector<std::string>& lines, 
	std::map<std::string, std::set<sz>>& wordMap,
	const std::string& queryWord){
	// lookup queryWord
	auto search = wordMap.find(queryWord);
	if (search == wordMap.end()){// not found.
		std::cout << queryWord << " not found." << std::endl;
	}	
	else{	// print result
		auto lineNumbers = search->second;
		std::cout << queryWord << " occurs in " << lineNumbers.size() << " line"
		<< ((lineNumbers.size() == 1) ? "": "s") << std::endl;
		for (auto num : lineNumbers)
			std::cout << "\t(line " << num + 1 << ") " 
				<< *(lines.begin() + num) << std::endl;
	}
}
void runQueries(std::istream& in){
	std::vector<std::string> lines;
	std::map<std::string, std::set<sz>> wordMap;
	generate_map(lines, wordMap, in);	// store the file and build the query map
		
	// iterate with the user: prompt for a word to find and print results
	while(true){
		std::cout << "Enter word to look for, or q to quit: ";
		std::string queryWord;
		// stop if we hit end-of-file on the input or if a 'q' is entered
		if (!(std::cin >> queryWord) || queryWord == "q") return;
		query(lines, wordMap, queryWord);
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
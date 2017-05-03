#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <functional>
#include <algorithm>

using std::string;
using map_type = std::map<string, string>;
map_type generate_map(std::istream& in){
	map_type tranform_map;
	string to_replace, replacement;
	while (in >> to_replace && getline(in, replacement))
		tranform_map.insert({to_replace, replacement.substr(1)});
	return tranform_map;
}
const string& replace(const string& word, const map_type& transform){
	auto found = transform.find(word);
	return (found == transform.end()) ? word: found->second;
} 
void output_tranform(std::istream& in, const map_type& transform){
	using namespace std::placeholders;
	string line, word;
	auto replacement = std::bind(replace, _1, transform);
	static std::ostream_iterator<string> out(std::cout, " " );
	while(getline(in, line)){
		std::istringstream iss(line);
		std::istream_iterator<string> is(iss), eof;
		std::transform(is, eof, out, replacement);
		std::cout << std::endl;
	}
}
int main(int argc, char** argv){
	if (argc < 3) {
		std::cerr << "Please specify transform and input files" << std::endl;
		std::exit(-1);
	}
	std::ifstream transform_file(argv[1]), input(argv[2]);
	if (!transform_file || !input){
		std::cerr << "Error opening files" << std::endl;
		std::exit(-1);
	}	
	output_tranform(input, generate_map(transform_file));
	return 0;
}



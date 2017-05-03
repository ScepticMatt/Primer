#include <set>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

void clean(std::string& word){
    word.erase(std::remove_if(word.begin(), word.end(), 
    	[](char ch){ return !::iswalnum(ch); }), word.end());
}
int main(int argc, char**argv){
	if (argc <2) return -1;
	const std::set<std::string> top100_english_words{"the", "be", "to", "of", "and", "a", "in", 
		"that", "have", "I", "it", "for", "not", "on", "with", "he", "as", "you", "do", "at", 
		"this", "but", "his", "by", "from", "they", "we", "say", "her", "she", "or", "an", "will", 
		"my", "one", "all", "would", "there", "their", "what", "so", "up", "out", "if", "about", 
		"who", "get", "which", "go", "me", "when", "make", "can", "like", "time", "no", "just", 
		"him", "know", "take", "people", "into", "year", "your", "good", "some", "could", "them", 
		"see", "other", "than", "then", "now", "look", "only", "come", "its", "over", "think", 
		"also", "back", "after", "use", "two", "how", "our", "work", "first", "well", "way", 
		"even", "new", "want", "because", "any", "these", "give", "day", "most", "us"};
	// input
	std::map<std::string, std::size_t> found;
	std::string word;
	std::fstream in(argv[1]);
	while (in >> word){
		clean(word);	// remove punctuations
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);
		if (top100_english_words.find(word) == top100_english_words.end())
			++found[word];
	}
	for (const auto& el : found)
		std::cout << el.first << ": " << el.second << " time" 
			<< ((el.second > 1)? "s" : "") << std::endl;
	return 0;
}
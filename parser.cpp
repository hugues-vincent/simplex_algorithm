#include <fstream>
#include <sstream>
#include <string>
#include <regex>

// containers
#include <vector>

// own classes
#include "tableau.h"
#include "parser.h"

using namespace std;

parser::parser(){}

vector<vector<string>> parser::matrix = {};
bool parser::parse_file(const string& file_path, tableau& table)
{
	
	if(!reader(file_path))
		return false;

	for(vector<string> row : matrix){
		for(string word : row)
			cout << word << ", ";
		cout<<"\n";
	}
	if(!standard_form(table))
		return false;
	return true;
}

bool parser::reader(const string& file_path)
{	
	ifstream file = ifstream(file_path);
	bool file_readable=true, max_defined=false;
	string line;	
	while(file_readable && getline(file, line))	
	{
	    vector<string> words;
	    line = regex_replace(line, regex("\\s"), "");
	    regex rgx_search("[^a-zA-Z\\.0-9+<>=\\-]"),
	    	rgx_iterator("([a-zA-Z]+)|([+-])|([0-9]+(\\.[0-9]+)?)|([<>]=?)");

	    // if there are forbid chars
	    if(regex_search(line, rgx_search))
		 	file_readable = false;

	    for(sregex_iterator it(line.begin(), line.end(), rgx_iterator), it_end; it != it_end; ++it)
	    {
	    	string word = it->str(),
    			last_word = (words.empty()) ? "" : words.back();
	    	regex is_number("[0-9]+(\\.[0-9]+)?"),
	    		is_letter("[a-zA-Z]"),
	    		is_sign("[+\\-]"),
	    		is_comparator("[<>]=?"),
	    		is_max("max");

	    	if(regex_match(word, is_number))
	    	{
	    		if(words.empty()
	    			|| regex_match(last_word, is_sign)
	    			|| regex_match(last_word, is_max)
	    			|| regex_match(last_word, is_comparator))
	    			words.push_back(word);
	    		else file_readable = false;
	    	}
	    	else if(regex_match(word, is_max))
	    	{
	    		if(!max_defined && words.empty())
	    		{
	    			words.push_back(word);
		    		max_defined = true;
	    		}
	    		else file_readable = false;
	    	}
	    	else if(regex_match(word, is_letter) && word != "rhs")
	    	{

	    		if(words.empty() || regex_match(last_word, is_sign) || regex_match(last_word, is_number))
	    			words.push_back(word);
	    		else file_readable = false;
	    	}
	    	else if(regex_match(word, is_sign))
	    	{
    			if(words.empty()
    				|| regex_match(last_word, is_letter)
    				|| regex_match(last_word, is_max)
    				|| regex_match(last_word, is_comparator))
    				words.push_back(word);
		    	else file_readable = false;

	    	}
	    	else if(regex_match(word, is_comparator))
	    	{
	    		if(regex_match(last_word, is_letter))
    				words.push_back(word);
	    		else file_readable = false;
	    	}
	    	else file_readable = false;
	    }
	    matrix.push_back(words);
	}
	return file_readable;
}
bool parser::standard_form(tableau& table)
{
	return true;
}


#include <iostream>
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

bool readFile(ifstream file, tableau& table)
{
	bool file_readable=true, max_defined=false;
	string line;
    vector<vector<string>> matrix;
	
	while(file_readable && getline(file, line))	
	{
	    vector<string> words;
	    line = regex_replace(line, regex("\\s"), "");
	    cout << line << "\n";
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

			cout << word << "\n";

	    	if(regex_match(word, is_number))
	    	{
	    		// cout << "---------is number\n";
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
	    cout << "\n";
	    // for each(string word in words)
	    // {
	    // 	cout << word << "\n";
	    // }
	}
	return file_readable;
}


int main(int argc, char *argv[])
{
	tableau t;
	tableau& table=t;
	vector<double> v{3, 9};
	table.add_row(vector<double> {3, 9}, set<string> {"x", "y"});
	table.add_row(vector<double> {1, 19});
	table.add_row(vector<double> {1, 19, 22});
	table.add_row(vector<double> {2, 1, 99}, set<string> {"z"});
	table.add_variable("lol");
	table.print();
	
	if(argc>1){
		// ifstream& file = ifstream(argv[1]);
		if(parser::parse_file(argv[1], table))
		{
			table.print();
		}	
		else cout << "file isn't readable\n"; 
	}
	else cout << "Can't reach the file\n";
	return 0; 
}



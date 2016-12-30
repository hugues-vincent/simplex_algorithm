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
vector<string> parser::col_names = {};
vector<double> parser::col_variables = {};
bool parser::parse_file(const string& file_path, tableau& table)
{
	
	if(!reader(file_path))
		return false;

    cout<<"\n";
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
	bool file_readable=true;
	string line;	
	while(file_readable && getline(file, line))	
	{
		if(file_readable = fills_vectors_from_file(line))
			fills_tableau_from_vectors();
	}
	return file_readable;
}
bool parser::standard_form(tableau& table)
{
	return true;
}
bool parser::fills_vectors_from_file(string& line)
{
	bool file_readable=true, max_defined=false;
    vector<string> words;
    line = regex_replace(line, regex("\\s"), "");
    regex rgx_search("[^a-zA-Z\\.0-9+<>=\\-]"),
    	rgx_iterator("([a-zA-Z]+)|([+-])|([0-9]+(\\.[0-9]+)?)|([<>]=?)");

    col_names = {};
    col_variables = {};
    // if there are forbidden chars
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
    		if(words.empty() || regex_match(last_word, is_max) || regex_match(last_word, is_comparator))
    		{
    			words.push_back(word);	
    			col_variables.push_back(stod(word));
    		}
    		else if(regex_match(last_word, is_sign))
    		{
    			words.push_back(word);
    			col_variables[col_names.size()] = col_variables.back() * stod(word);
    		}
    		else file_readable = false;
    	}
    	else if(regex_match(word, is_max))
    	{
    		if(!max_defined && words.empty())
    		{
    			words.push_back(word);
	    		col_names.push_back("max");
	    		col_variables.push_back(1.);
	    		max_defined = true;
    		}
    		else file_readable = false;
    	}
    	else if(regex_match(word, is_letter) && word != "rhs")
    	{

    		if(words.empty())
    		{
    			words.push_back(word);
    			col_names.push_back(word);
    			col_variables.push_back(1.);
    		}
    		else if(regex_match(last_word, is_sign) || regex_match(last_word, is_number))
    		{
    			words.push_back(word);
    			col_names.push_back(word);
    		}
    		else file_readable = false;
    	}
    	else if(regex_match(word, is_sign))
    	{
			if(words.empty()
				|| regex_match(last_word, is_letter)
				|| regex_match(last_word, is_max)
				|| regex_match(last_word, is_comparator))
			{
				words.push_back(word);
				col_variables.push_back((regex_match(word, regex("\\+"))) ? 1. : -1.);
			}
	    	else file_readable = false;
    	}
    	else if(regex_match(word, is_comparator))
    	{
    		if(regex_match(last_word, is_letter))
    		{
				words.push_back(word);
    			col_names.push_back("rhs");
    		}
    		else file_readable = false;
    	}
    	else file_readable = false;
    }
    matrix.push_back(words);
    return file_readable;
}
bool parser::fills_tableau_from_vectors()
{
    cout<<"\n";
    for(string col_name : col_names) cout << col_name<<", ";
    cout<<"\n";
    for(double col_variable : col_variables) cout << col_variable<<", ";
    cout<<"\n";
	return true;
}


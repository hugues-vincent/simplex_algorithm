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
vector<vector<string>> parser::col_names = {};
vector<vector<double>> parser::col_variables = {};
tableau foo;
tableau& parser::tab = foo;
int parser::nb_var = 0;


bool parser::parse_file(const string& file_path, tableau& table)
{
	tab = table;
	if(!reader(file_path))
		return false;

 //    cout<<"\n";
	// for(vector<string> row : matrix){
	// 	for(string word : row)
	// 		cout << word << ", ";
	// 	cout<<"\n";
	// }
    // 
    fills_tableau_from_vectors();
	if(!standard_form())
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
		file_readable = fills_vectors_from_file(line);
    }
	return file_readable;
}
bool parser::standard_form()
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

    vector<string> line_col_names = {};
    vector<double> line_col_variables = {};
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
    			line_col_variables.push_back(stod(word));
    		}
    		else if(regex_match(last_word, is_sign))
    		{
    			words.push_back(word);
    			line_col_variables[line_col_names.size()] = line_col_variables.back() * stod(word);
    		}
    		else file_readable = false;
    	}
    	else if(regex_match(word, is_max))
    	{
    		if(!max_defined && words.empty())
    		{
    			words.push_back(word);
	    		line_col_names.push_back("max");
	    		tab.add_variable("max");
	    		line_col_variables.push_back(1.);
	    		max_defined = true;
    		}
    		else file_readable = false;
    	}
    	else if(regex_match(word, is_letter) && word != "rhs")
    	{

    		if(words.empty())
    		{
    			words.push_back(word);
    			line_col_names.push_back(word);
    			tab.add_variable(word);
    			line_col_variables.push_back(1.);
    		}
    		else if(regex_match(last_word, is_sign) || regex_match(last_word, is_number))
    		{
    			words.push_back(word);
    			line_col_names.push_back(word);
    			tab.add_variable(word);
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
				line_col_variables.push_back((regex_match(word, regex("\\+"))) ? 1. : -1.);
			}
	    	else file_readable = false;
    	}
    	else if(regex_match(word, is_comparator))
    	{
    		if(regex_match(last_word, is_letter))
    		{
				words.push_back(word);
    			line_col_names.push_back("rhs");
    			tab.add_variable("rhs");
    		}
    		else file_readable = false;
    	}
    	else file_readable = false;
    }
    matrix.push_back(words);
    col_names.push_back(line_col_names);
    col_variables.push_back(line_col_variables);
    cout<<"\n";
    for(string word : words) cout << word<<", ";
    cout<<"\n";
    for(string col_name : line_col_names) cout << col_name<<", ";
    cout<<"\n";
    for(double col_variable : line_col_variables) cout << col_variable<<", ";
    cout<<"\n";

    return file_readable;
}
bool parser::fills_tableau_from_vectors()
{
	vector<double> row;
    for(int j=0; j<col_names.size() ;j++)
    {
    	row = {};
        for(int i= 0; i<tab.get_nb_var() ; i++)
    		row.push_back(0.);
    	for(int i=0 ; i<col_names[j].size() ; i++)
    	{
    		int var_position = tab.add_variable(col_names[j][i]);
    		row[var_position] = col_variables[j][i];
    	}

    	tab.add_row(row);
    	tab.print();
    	cout<< "\n";
    }
	return true;
}


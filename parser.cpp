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
vector<comparator> parser::comparators = {};
int parser::nb_var = 0;


bool parser::parse_file(const string& file_path, tableau& tab)
{
	if(!reader(file_path, tab))
		return false;
    if(!fills_tableau_from_vectors(tab))
        return false;
    return true;
}
bool parser::reader(const string& file_path, tableau& tab)
{	
	ifstream file = ifstream(file_path);
	bool file_readable=true;
	string line;	
	while(file_readable && getline(file, line))	
	{
		file_readable = fills_vectors_from_line(line, tab);
    }
	return file_readable;
}
bool parser::fills_vectors_from_line(string& line, tableau& tab)
{
	bool file_readable=true, max_defined=false;
    vector<string> words;
    line = regex_replace(line, regex("\\s"), "");
    regex rgx_search("[^a-zA-Z\\.0-9+<>=\\-]"),
    	rgx_iterator("([a-zA-Z]+)|([+-])|([0-9]+(\\.[0-9]+)?)|([<>]=?)|(=)");

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
    		is_comparator("([<>]=?)|(=)"),
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
                if(regex_match(words[words.size()-3], is_comparator))
                    line_col_variables[line_col_names.size()-1] = line_col_variables.back() * stod(word);
                else 
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
                comparators.push_back(EQUAL);
    		
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
                if(regex_match(word, regex("[<]=?"))) comparators.push_back(INFERIOR);
                else if(regex_match(word, regex("[>]=?"))) comparators.push_back(SUPERIOR);
                else if(regex_match(word, regex("="))) comparators.push_back(EQUAL);
    		}
    		else file_readable = false;
    	}
    	else file_readable = false;
    }
    matrix.push_back(words);
    col_names.push_back(line_col_names);
    col_variables.push_back(line_col_variables);
    return file_readable;
}
// comparator get_line_comparator()
bool parser::fills_tableau_from_vectors(tableau& tab)
{
	vector<double> row;
    for(int j=0; j<col_names.size() ;j++)
    	tab.add_row(col_variables[j], col_names[j] ,comparators[j]);

    // rearrange the tableau in order to have:
    // 1- the objective function at the bottom of the tableau 
    tab.swap_row(tab.get_base_row_positon("max"), tab.get_nb_row() - 1);
    tab.remove_variable("max");
    // 2- the constant value at the right of the tableau
    tab.swap_col(tab.get_var_position("rhs"), tab.get_nb_var() - 1);
    cout << "\n";
    tab.print(true);
    cout << "\n";
	return true;
}

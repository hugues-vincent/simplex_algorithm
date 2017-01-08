#include "tableau.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <iomanip>

using namespace std;

void nl(int k){ int j; for(j=0;j<k;j++) putchar('-'); putchar('\n'); }

tableau::tableau()
{
    // vector<double> v1{3, 3, 9}, v2{5}, v3{31};
    // vector<vector<double>> foo = {v1, v2, v3};
    // matrix = foo;
    // variables = set<string>();
}

int tableau::get_var_position(string var_name) const
{
	int i=0, position , nb_var=variables.size();
	while(i<nb_var)
	{
		if(variables[i] == var_name)
		{
			position = i;
			i = nb_var;
		}
		else i++;
	}
	return position;
}
int tableau::get_nb_var() const { return variables.size(); }

int tableau::add_variable(string var_name)
{
	// int position = 0;
	set<string> set(variables.begin(), variables.end());
	if(set.insert(var_name).second)
	{
		variables.push_back(var_name);
		for(vector<double>& row : matrix)
			row.push_back(0.);
		return variables.size() - 1;
	}
	else 
	{
		for(int i=0; i<variables.size(); i++)
			if(variables[i] == var_name)
				return i;
	} 
}

bool tableau::add_row(vector<double> row)
{
	bool is_added = false;
	if(variables.size() == row.size())
	{
		matrix.push_back(row); is_added = true; } return is_added; }

bool tableau::add_row(vector<double> row, set<string> var_names)
{
	for(string var_name : var_names)
		tableau::add_variable(var_name);
    return add_row(row);
}
void tableau::print() const
{
    nl(8*matrix.front().size());
    for(string variable : variables)
    	cout << left << setw(8) << variable;
    cout << '\n';
    nl(8*matrix.front().size());
    for(vector<double> row : matrix) {
        for(double val : row) {
            cout << left << setw(8) << val;
        }
        cout << '\n';
    }
    nl(8*matrix.front().size());
}
void tableau::print(const string msg) const
{
	cout << "\n - " << msg << "\n";
	tableau::print();
}

#include "tableau.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

void nl(int k){ int j; for(j=0;j<k;j++) putchar('-'); putchar('\n'); }

tableau::tableau()
{
    // vector<double> v1{3, 3, 9}, v2{5}, v3{31};
    // vector<vector<double>> foo = {v1, v2, v3};
    // matrix = foo;
    // variables = set<string>();
}
int tableau::get_base_row_positon(string var_name) const
{
	int var_position, row_position;
	var_position = get_var_position(var_name);
	for(int i=0; i<matrix.size(); i++)
	{
		if(matrix[i][var_position] == 1. && row_position != -1) row_position = i;
		else if(matrix[i][var_position] != 0.) row_position = -1;
	}
	return row_position;
}
int tableau::get_var_position(string var_name) const
{
	int i, position=-1 , nb_var=variables.size();
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
int tableau::get_nb_row() const { return matrix.size(); }

int tableau::add_variable(string var_name)
{
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
bool tableau::remove_variable(string var_name)
{
	int position;
	position = get_var_position(var_name);
	if(position == -1) return false;
	variables.erase(variables.begin() + position);
	for(vector<double>& row : matrix)
		row.erase(row.begin() + position);
	return true;
}
bool tableau::add_row(vector<double> row, comparator comp)
{
	bool is_added = false;
	if(variables.size() == row.size())
	{
		matrix.push_back(row);
		comparators.push_back(comp);
		is_added = true;
	} 
	return is_added;
}

bool tableau::add_row(vector<double> row, vector<string> var_names, comparator comp)
{
	vector<double> tmp_row = {};
 	set<string> set_names(var_names.begin(), var_names.end());
 	if(set_names.size() != var_names.size() || row.size() != var_names.size())
 		return false;
    for(int i= 0; i<get_nb_var() ; i++)
		tmp_row.push_back(0.);
	for(int i=0 ; i<var_names.size() ; i++)
	{
		int var_position = add_variable(var_names[i]);
		// if(var_position >= get_nb_var()) tmp_row.push_back(row[i]);
		tmp_row[var_position] = row[i];
	}
    return add_row(tmp_row, comp);
}

bool tableau::add_slacks()
{
	if(comparators.size() != matrix.size()) return false;
	// for(int i=0; )
	return true;	
}
bool tableau::swap_col(int pos1, int pos2)
{
	if(!(pos1 < variables.size() && pos2 < variables.size())) return false;
	swap(variables[pos1], variables[pos2]);
	for(vector<double>& row : matrix)
		swap(row[pos1], row[pos2]);
	return true;
}
bool tableau::swap_row(int pos1, int pos2)
{
	if(!(pos1 < matrix.size() && pos2 < matrix.size())) return false;
	swap(matrix[pos1], matrix[pos2]);
	swap(comparators[pos1], comparators[pos2]);
	return true;
}

void tableau::print() const
{
    nl(8*matrix.front().size() + 8);
    int size = variables.size();
    for(int i=0; i<size; i++)
    {
    	if(i == size - 1) cout << left << setw(8) << " ";
    	cout << left << setw(8) << variables[i];
    }
    cout << '\n';
    nl(8*matrix.front().size() + 8);
    for(int j=0; j<matrix.size(); j++) 
    {
    	for(int i=0; i<size; i++)
        {
            if(i == size - 1)
            {
            	switch(comparators[j])
            	{
            		case 0 : {
						cout << left << setw(8) << "=";
            			break;
            		}
            		case 1 : {
						cout << left << setw(8) << "<";
            			break;
            		}
            		case 2 : {
						cout << left << setw(8) << ">";
            			break;
            		}
            	}
            }
            cout << left << setw(8) << matrix[j][i];
        }
        cout << '\n';
    }
    nl(8*matrix.front().size() + 8);
}
void tableau::print(const string msg) const
{
	cout << "\n - " << msg << "\n";
	tableau::print();
}

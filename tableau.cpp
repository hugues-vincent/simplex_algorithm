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

tableau::tableau(){}
tableau::tableau(vector<vector<double>> _matrix, vector<string> _variables, vector<comparator> _comparators)
{
	matrix = _matrix;
	variables = _variables;
	comparators = _comparators;
}
tableau tableau::operator=(const tableau& other)
{
	return tableau(other.get_matrix(), other.get_variables(), other.get_comparators());
}
// getters
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
vector<vector<double>> tableau::get_matrix() const { return matrix; }
vector<string> tableau::get_variables() const { return variables; }
vector<comparator> tableau::get_comparators() const { return comparators; }
// modfiers
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
<<<<<<< HEAD
bool tableau::add_row(vector<double> row, vector<string> var_names, comparator comp)
{
	bool is_added = false;
=======
bool tableau::add_constaint(vector<double> row, comparator comp)
{
	matrix.push_back(row);
	swap(matrix[get_nb_row()-1], matrix[get_nb_row() - 2]);
	comparators.push_back(comp);
	swap(comparators[get_nb_row()-1],comparators[get_nb_row() - 2]);
}
bool tableau::add_row(vector<double> row, vector<string> var_names, comparator comp)
{
>>>>>>> master2
	int var_position, col_rhs;
	vector<double> tmp_row(get_nb_var(), 0.);
 	set<string> set_names(var_names.begin(), var_names.end());
 	if(set_names.size() != var_names.size() || row.size() != var_names.size())
 		return false;
	for(int i=0 ; i<var_names.size() ; i++)
	{
		var_position = add_variable(var_names[i]);
		tmp_row[var_position] = row[i];
		if(var_names[i] == "rhs") col_rhs = var_position; 
	}
	if(tmp_row[col_rhs] < 0)
	{
		for(double& val: tmp_row) val = -val;
		if(comp == INFERIOR) comp = SUPERIOR;
		else if(comp == SUPERIOR) comp = INFERIOR;
	}
	matrix.push_back(tmp_row);
	comparators.push_back(comp);
    return true;
}
set<int> tableau::add_slacks()
{
	set<int> artificial_variables;
	int nb_artificial_var = 0, new_var_col;
	for(int i=0; i<matrix.size()-1; i++)
	{
		switch(comparators[i])
    	{
			// if equal
    		case 0 : {
				new_var_col = add_variable("a" + to_string(nb_artificial_var));
    			matrix[i][new_var_col] = 1.;
				nb_artificial_var ++;
    			swap_col(new_var_col, new_var_col-1);
    			artificial_variables.insert(new_var_col-1);
    			break;
    		}
			// if inferior
    		case 1 : {
				new_var_col = add_variable("s" + to_string(i));
    			matrix[i][new_var_col] = 1.;
    			swap_col(new_var_col, new_var_col-1);
    			break;
    		}
			// if superior
    		case 2 : {
				new_var_col = add_variable("s" + to_string(i));
    			matrix[i][new_var_col] = -1.;
    			swap_col(new_var_col, new_var_col-1);
				new_var_col = add_variable("a" + to_string(nb_artificial_var));
    			matrix[i][new_var_col] = 1.;
    			swap_col(new_var_col, new_var_col-1);
    			artificial_variables.insert(new_var_col-1);
				nb_artificial_var ++;
    			break;
    		}
    	}
    	comparators[i] = EQUAL;
	}
	return artificial_variables;	
}
int tableau::find_pivot_column() const
{
	int pivot_col = 0;
	double highest = matrix.back()[pivot_col];
	for(int j=1; j<get_nb_var()-1; j++)
	{
	  if(matrix.back()[j] > highest)
	  {
	    highest = matrix.back()[j];
	    pivot_col = j;
	  }
	}
	if(highest <= 0) return -1; // All negative columns in objctive function, this is optimal.
	cout << "pivot_col: " << pivot_col << ", ";
	return pivot_col;
}
int tableau::find_pivot_row(int pivot_col) const
{
	int pivot_row = 0;
	double min_ratio = -1., ratio;
	double min = matrix.back()[pivot_col];
	for(int j=0; j<matrix.size()-1; j++)
	{
	  ratio = matrix[j][get_nb_var()-1] / matrix[j][pivot_col];
	  if((ratio > 0 && ratio < min_ratio) || min_ratio < 0)
	  {
	    min_ratio = ratio;
	    pivot_row = j;
	  }
	}
	if(min_ratio == -1) return -1;
	cout << "pivot_row: " << pivot_row << "\n";
	return pivot_row;
}
void tableau::pivot_on(int pivot_col, int pivot_row)
{
	double multiplier;
	multiplier = matrix[pivot_row][pivot_col];
	for(int i=0; i<matrix[0].size(); i++)
		matrix[pivot_row][i] /= multiplier;
	for(int i=0; i<matrix.size(); i++)
	{
		multiplier = matrix[i][pivot_col];
		if(i == pivot_row) continue;
		for(int j=0; j<matrix[0].size(); j++)
			matrix[i][j] -= multiplier * matrix[pivot_row][j];
	}
}
bool tableau::simplex()
{
	int loop=0;

	while( ++loop )
	{
		int pivot_col, pivot_row;

		pivot_col = find_pivot_column();
		if(pivot_col < 0) break;

		pivot_row = find_pivot_row(pivot_col);
		if (pivot_row < 0) break;

		pivot_on(pivot_col, pivot_row);
		// print();

		if(loop > 20) break;  
	}
}
/**
 * alow to make simplex computation on rows not defined in the tableau
 * @param  last_objective_function: 
 */
bool tableau::simplex(vector<double>& outside_row)
{
	int loop=0;

	while( ++loop )
	{
		int pivot_col, pivot_row;

		pivot_col = find_pivot_column();
		if(pivot_col < 0) break;

		pivot_row = find_pivot_row(pivot_col);
		if (pivot_row < 0) break;

		pivot_on(pivot_col, pivot_row);
		double factor = outside_row[pivot_col];
		for(int i=0; i<get_nb_var(); i++)
			outside_row[i] -= factor * matrix[pivot_row][i];
		// print();

		if(loop > 20) break;  
	}
}
void tableau::change_objective_function(vector<double>& new_objective_function, vector<double>& old_objective_function)
{
	old_objective_function = {};
	for(double v : matrix.back())
		old_objective_function.push_back(v);
	matrix[matrix.size() - 1] = new_objective_function;
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
bool tableau::remove_variable(int var_col)
{
	variables.erase(variables.begin() + var_col);
	for(vector<double>& row : matrix)
		row.erase(row.begin() + var_col);
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

// displayers
void tableau::print(const bool show_comparator) const
{
    int shift_line = (show_comparator)?8:0;
    nl(8*matrix.front().size() + shift_line);
    int size = variables.size();
    for(int i=0; i<size; i++)
    {
    	if(i == size - 1 && show_comparator) cout << left << setw(8) << " ";
    	cout << left << setw(8) << variables[i];
    }
    cout << '\n';
    nl(8*matrix.front().size() + shift_line);
    for(int j=0; j<matrix.size(); j++) 
    {
    	for(int i=0; i<size; i++)
        {
            if(i == size - 1 && show_comparator)
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
    nl(8*matrix.front().size() + shift_line);
}
void tableau::print(const string msg, const bool show_comparator) const
{
	cout << "\n - " << msg << "\n";
	tableau::print(show_comparator);
}

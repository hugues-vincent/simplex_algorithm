#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <numeric>
#include <iomanip>
#include <math.h> 
// containers
#include <vector>

// own classes
#include "tableau.h"
#include "parser.h"

using namespace std;

vector<double> lp_full(tableau& tab, bool display_result = false)
{
	vector<double> solutions = {};
	vector<vector<double>> mat = tab.get_matrix();
	int nb_initial_var = tab.get_nb_var() -1; 
	set<int> artificial_variables;
	artificial_variables = tab.add_slacks();
	if(artificial_variables.empty())
	{
		if(display_result) cout << "One phase simplex\n";
		if(display_result) tab.print(false);
		tab.simplex();
		if(display_result) tab.print(false);

	}
	else
	{
		cout << "Two phase simplex\n";
		int base_row;
		vector<vector<double>> mat = tab.get_matrix();
		vector<double> u(tab.get_nb_var(), 0.), z;
		for(int artificial_variable : artificial_variables)
		{
			u[artificial_variable] = -1.;
			for(int i=0; i<tab.get_nb_row(); i++)
				if(mat[i][artificial_variable] == 1) base_row = i;
			for(int i=0; i<tab.get_nb_var(); i++)
				u[i] += mat[base_row][i];
		}		
		tab.change_objective_function(u,z);
		if(display_result) tab.print("1st phase", false);
		tab.simplex(z);
		if(display_result) tab.print();
		if(tab.get_matrix().back().back() != 0) 
		{
			cout << "there are no feasible solutions to this system\n";
			return vector<double>{};
		}
		tab.change_objective_function(z, u);
		for(int artificial_variable : artificial_variables)
			tab.remove_variable(artificial_variable);
		if(display_result) tab.print("2nd phase", false);
		tab.simplex();
		if(display_result) tab.print(false);
	}
	// get optimum vertex coordonate
	int base_row = 0;
	for(int i=0; i<nb_initial_var; i++)
	{
		base_row = -1;
		for(int j=0; j<tab.get_matrix().size(); j++)
		{
			if(tab.get_matrix()[j][i] == 1 && base_row == -1) base_row = j;
			else if(tab.get_matrix()[j][i] != 0)
			{
				base_row = -1;
				break;
			}
		}
		if(base_row == -1) solutions.push_back(0.);
		else solutions.push_back(tab.get_matrix()[base_row].back());
	}
	if(display_result)
	{
		cout << "The solution for the lp full system is:\n";
		for(int i=0; i<nb_initial_var; i++)
	    	cout << left << setw(8) << tab.get_variables()[i];
		cout << "\n";
		for(double solution : solutions)
	    	cout << left << setw(8) << solution;
		cout << "\n";
	}

	return solutions;
}
void lp_integer(tableau& tab)
{
	// tableau tab_lower_constraint, tab_higher_constraint;
	vector<vector<double>> tree_solutions;
	vector<double> solutions;

	vector<tableau> tree_tableau;
	int nb_initial_var = tab.get_nb_var() -1; 
	int current_node = 0;
	tree_tableau.push_back(tab);
	while(current_node < tree_tableau.size() && current_node < 30)
	{
		if(!tree_tableau[current_node].get_matrix().empty())
		{
			tableau tab_lower_constraint = tree_tableau[current_node];
			tableau tab_higher_constraint = tree_tableau[current_node];
			solutions = lp_full(tree_tableau[current_node]);
			if(!solutions.empty())
			{
				for(int i=0; i<solutions.size(); i++)
				{
					if(solutions[i] != floor(solutions[i]))
					{
						vector<double> tmp_vector(tab.get_nb_var(), 0.);
						tmp_vector[i] = 1;
						tmp_vector.back() = floor(solutions[i]);
						// for(double v : tmp_vector) cout << v <<", ";
						// cout<<"\n" << tab.get_variables().size()<<", "<< tab.get_nb_var()<<"\nlol\n";
						tab_lower_constraint.add_constaint(tmp_vector, INFERIOR);
						// cout <<"lol\n";
						// cout <<"lol0\n";
						tree_tableau.push_back(tab_lower_constraint);
						// cout <<"lol2\n";
						
						tmp_vector.back() += 1;
						tab_higher_constraint.add_constaint(tmp_vector, SUPERIOR); 
						tree_tableau.push_back(tab_higher_constraint);
						break;
					}
					// if all solutions are integer
					else if(solutions[i] == solutions.back()) 
						tree_solutions.push_back(solutions);
				}
			}
		}
		current_node ++;
	}
	double result = 0, tmp_result = 0;
	vector<double> optimum_vertex;
	if(tree_solutions.empty()) { cout<< "there is no integral solution for this system\n"; return; }
	for(vector<double> solution : tree_solutions)
	{
		tmp_result = 0;
		for(int i=0; i<solution.size(); i++)
			tmp_result += tab.get_matrix().back()[i] * solution[i];
		if(tmp_result > result)
		{
		 	result = tmp_result;
			optimum_vertex = solution;
		}
	}
	cout << "The solution for the lp integral system is:\n";
	for(int i=0; i<nb_initial_var; i++)
    	cout << left << setw(8) << tab.get_variables()[i];
	cout << "\n";
	for(double optimum : optimum_vertex)
    	cout << left << setw(8) << optimum;
	cout << "\n";
}
void lp_binary(tableau& tab)
{
	double z, z_0, z_1;
	double tmp_double = 0;
	bool feasible_0, feasible_1;
	vector<double> thetas, thetas_0, thetas_1;
	vector<double> tmp_vector, solutions;
	vector<vector<double>> matrix;
	vector<string> variables = tab.get_variables();
  	vector<comparator> comparators = tab.get_comparators();

	// initialize z at the root node
	matrix = tab.get_matrix();
	for(double val : matrix.back())
	{
		if(val > 0) z += val;
	}
	// initialize thetas at the root node
	for(int i=0; i<matrix.size()-1; i++)
	{
		tmp_vector = matrix[i];
		if(comparators[i] == INFERIOR)
		{
			tmp_double = tmp_vector.back();
			for(int j=0; j<matrix[0].size()-1; j++)
				if(tmp_vector[j] < 0)
					tmp_double -= tmp_vector[j];
			thetas.push_back(tmp_double);
		}
		else if(comparators[i] == SUPERIOR)
		{
			for(int j=0; j<matrix[0].size()-1; j++)
				if(tmp_vector[j] > 0)
					tmp_double += tmp_vector[j];
			thetas.push_back(tmp_double);
			tmp_double -= tmp_vector.back();
		}
	}
	// run the algorith through the binary tree
	for(int i=0; i<matrix[0].size()-1; i++)
	{
		z_0 = z;
		z_1 = z;
		thetas_0 = thetas;
		thetas_1 = thetas;
		feasible_0 = true;
		feasible_1 = true;
		// z_k+1
		tmp_double = matrix.back()[i];
		z_0 -= (tmp_double > 0) ? tmp_double : 0;
		z_1 += (tmp_double < 0) ? tmp_double : 0;
		// theta_k+1
		for(int j=0; j<matrix.size()-1; j++)
		{
			tmp_double = matrix[j][i];
			if(comparators[i] == INFERIOR)
			{
				// x_k+1 <- 0
				thetas_0[j] += (tmp_double <= 0) ? tmp_double : 0;
				if(thetas_0[j] < 0) feasible_0 = false;
				// x_k+1 <- 1
				thetas_1[j] -= (tmp_double > 0) ? tmp_double : 0;
				if(thetas_1[j] < 0) feasible_1 = false;
			}
			if(comparators[i] == SUPERIOR)
			{
				// x_k+1 <- 0
				thetas_0[j] += (tmp_double <= 0) ? 0 : tmp_double;
				if(thetas_0[j] < 0) feasible_0 = false;
				// x_k+1 <- 1
				thetas_1[j] -= (tmp_double > 0) ? 0 : tmp_double;
				if(thetas_1[j] < 0) feasible_1 = false;
			}

		}
		if(!feasible_0 && !feasible_1)
		{
			cout << "no feasible solution for this binary system\n";
			return;
		}
		else if(!feasible_0 || (z_1 > z_0 && feasible_1))
		{
			z = z_1;
			thetas = thetas_1;
			solutions.push_back(1.);
		}
		else
		{
			z = z_0;
			thetas = thetas_0;
			solutions.push_back(0.);
		}
	}
	cout << "The solution for the binary system is:\n";
	for(int i=0; i<variables.size()-1; i++)
    	cout << left << setw(8) << variables[i];
	cout << "\n";
	for(double solution : solutions)
    	cout << left << setw(8) << solution;
	cout << "\n";
}
int main(int argc, char *argv[])
{
	tableau table;
	
	if(argc>1)
	{
		if(!parser::parse_file(argv[1], table)) cout << "file isn't readable\n";
		tableau table_binary = table;
		tableau table_integer = table;
		cout << "Simplex\n";
		lp_full(table, true);
		cout << "\nInteger\n";
		lp_integer(table_integer);
		cout << "\nBinary\n";
		lp_binary(table_binary);
	}
	else cout << "Can't reach the file\n";
	return 0; 
}



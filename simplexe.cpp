#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <numeric>
#include <iomanip>

// containers
#include <vector>

// own classes
#include "tableau.h"
#include "parser.h"

using namespace std;

void lp_full(tableau& tab)
{
	bool is_one_phase_simplex;
	is_one_phase_simplex = tab.add_slacks();
	tab.print("added slacks", true);
	if(is_one_phase_simplex)
	{
		cout << "1 phase simplex\n";
	}
	else
	{
		cout << "2 phase simplex\n";
	}

}
void lp_integer()
{
	
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
	cout << "the solution for the binary system is:\n";
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
		lp_full(table);
		// lp_integer(table);
		// lp_binary(table);
	}
	else cout << "Can't reach the file\n";
	return 0; 
}



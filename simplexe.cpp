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
void lp_full()
{

}
void lp_integer()
{
	
}
void lp_binary(tableau& tab)
{
	double z, z_0, z_1;
	double tmp_double;
	vector<double> thetas, thetas_0, thetas_1;
	vector<double> tmp_vector, solutions;
	bool feasible_0, feasible_1;
	vector<vector<double>> matrix;
	vector<string> variables = tab.get_variables();

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
		tmp_double = tmp_vector.back();
		for(int j=0; j<matrix[0].size()-1; j++)
			if(tmp_vector[j] < 0)
				tmp_double -= tmp_vector[j];
		thetas.push_back(tmp_double);
	}
	for(int i=0; i<matrix[0].size()-1; i++)
	{
		z_0 = z;
		z_1 = z;
		thetas_0 = thetas;
		thetas_1 = thetas;
		feasible_0 = true;
		feasible_1 = true;
		// z_k+1
		// x_k+1 <- 0
		tmp_double = matrix.back()[i];
		z_0 -= (tmp_double > 0) ? tmp_double : 0;
		// x_k+1 <- 1
		z_1 += (tmp_double < 0) ? tmp_double : 0;
		// theta_k+1
		for(int j=0; j<matrix.size()-1; j++)
		{
			tmp_double = matrix[j][i];
			// x_k+1 <- 0
			thetas_0[j] += (tmp_double < 0) ? tmp_double : 0;
			if(thetas_0[j] < 0) feasible_0 = false;
			// x_k+1 <- 1
			thetas_1[j] -= (tmp_double > 0) ? tmp_double : 0;
			if(thetas_1[j] < 0) feasible_1 = false;
		}
		cout << "\n";
		cout << "z_0: "<< z_0 << "\n";
		for(double val : thetas_0)
			cout << ""<< val << "\n";
		cout << "z_1: "<< z_1 << "\n";
		for(double val : thetas_1)
			cout << ""<< val << "\n";
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
void cartesian(vector<vector<int>>& v,tableau& tab)
{
	vector<vector<double>> matrix = tab.get_matrix();
  	vector<comparator> comparators = tab.get_comparators();
	int nb_var = tab.get_nb_var();
	vector<int> feasible_solution = {};
	double row_sum;
	bool is_feasible;
	auto product = []( long long a, vector<int>& b ) { return a*b.size(); };
	const long long N = accumulate( v.begin(), v.end(), 1LL, product );
	vector<int> u(v.size());
	for( long long n=0 ; n<N ; ++n )
	{
	    lldiv_t q { n, 0 };
	    for( long long i=v.size()-1 ; 0<=i ; --i )
	    {
	      q = div( q.quot, v[i].size() );
	      u[i] = v[i][q.rem];
		}
	    // Do what you want here with u.
		is_feasible = true;
    		cout <<"\n";
	    for(int i=0; i<matrix.size()-1; i++)
	    {
			row_sum = 0;
	    	for(int j=0; j<nb_var-1; j++)
	    	{
	    		row_sum += matrix[i][j] * u[j];
	    		cout << u[j] <<" ";
	    	}
    		cout << "= " << row_sum <<"\n";
        	switch(comparators[i])
            	{
            		case 0 : {
						if( row_sum != matrix[i][nb_var-1]) is_feasible = false;
            			break;
            		}
            		case 1 : {
						if( row_sum > matrix[i][nb_var-1]) is_feasible = false;
            			break;
            		}
            		case 2 : {
						if( row_sum < matrix[i][nb_var-1]) is_feasible = false;
            			break;
            		}
            	}
            if(!is_feasible) break;
	    }
    	row_sum = 0;
    	double tmp = 0;
    	if(feasible_solution.empty() && is_feasible)
    		feasible_solution = u;
    	else if(is_feasible)
    	{
	    	for(int i=0; i<nb_var-1; i++)
	    	{
				row_sum += matrix[matrix.size()-1][i] * u[i];
				tmp += matrix[matrix.size()-1][i] * feasible_solution[i];
	    	}
		    if(is_feasible && row_sum > tmp ) feasible_solution = u;
    	}

  	}
  	if(feasible_solution.empty())
		cout << "no solution";
	else 
	{
		for(int val : feasible_solution) cout<< val << " ";
	}
	cout << "\n";
}
int main(int argc, char *argv[])
{
	tableau table;
	
	if(argc>1)
	{
		if(!parser::parse_file(argv[1], table)) cout << "file isn't readable\n";
		lp_binary(table);
	}
	else cout << "Can't reach the file\n";
	vector<vector<int> > v(3, vector<int>{0, 1});
	cartesian(v, table);
	// vector<vector<double>> v(vector<double> {1, 2, 3}, 3);
	// for(vector<double> row : v)
	// {
	// 	for(double val: row)
	// 		cout << val << ", ";
	// 	cout <<"\n";		
	// }	

	return 0; 
}



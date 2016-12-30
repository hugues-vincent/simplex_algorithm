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

bool tableau::add_variable(string var_name)
{
	bool is_added = false;
	set<string> set(variables.begin(), variables.end());
	if(set.insert(var_name).second)
	{
		variables.push_back(var_name);
		for(vector<double>& row : matrix)
			row.push_back(0.);
		is_added = true;
	}
	return is_added;
}

bool tableau::add_row(vector<double> row)
{
	bool is_added = false;
	if(variables.size() == row.size())
	{
		matrix.push_back(row);
		is_added = true;
	}
	return is_added;
}

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

  // static int counter=0;
  // int i, j;
  // nl(70);

  // printf("%-6s%5s", "col:", "b[i]");
  // for(j=1;j<tab->n; j++) { printf("    x%d,", j); } printf("\n");

  // for(i=0;i<tab->m; i++) {
  //   if (i==0) printf("max:"); else
  //   printf("b%d: ", i);
  //   for(j=0;j<tab->n; j++) {
  //     if (equal((int)tab->mat[i][j], tab->mat[i][j]))
  //       printf(" %6d", (int)tab->mat[i][j]);
  //     else
  //       printf(" %6.2lf", tab->mat[i][j]);
  //     }
  //   printf("\n");
  // }
  // nl(70);

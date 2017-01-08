#ifndef TABLEAU_H
#define TABLEAU_H

#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

enum line_relation { equal, inferior, superior };

class tableau
{
public:
  tableau();

  int add_variable(string var_name);
  bool add_row(vector<double> row);
  bool add_row(vector<double> row, set<string> var_names);

  int get_var_position(string var_name) const;
  int get_nb_var() const;
  void print() const;
  void print(const string msg) const;

private: 
  vector<vector<double>> matrix;
  vector<string> variables;

};

#endif

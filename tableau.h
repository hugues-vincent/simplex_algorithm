#ifndef TABLEAU_H
#define TABLEAU_H

#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

enum comparator {EQUAL, INFERIOR, SUPERIOR};

class tableau
{
public:
  tableau();
  tableau(vector<vector<double>> matrix, vector<string> variables, vector<comparator> comparators);

  tableau operator=(const tableau& other);

  // getters
  int get_base_row_positon(string var_name) const;
  int get_var_position(string var_name) const;
  int get_nb_var() const;
  int get_nb_row() const;
  vector<vector<double>> get_matrix() const;
  vector<string> get_variables() const;
  vector<comparator> get_comparators() const;

  // modfiers
  int add_variable(string var_name);
  bool add_row(vector<double> row, vector<string> var_names, comparator comp = EQUAL);
  bool add_constaint(vector<double> row, comparator comp);
  set<int> add_slacks();
  bool simplex();
  bool simplex(vector<double>& outside_row);
  void change_objective_function(vector<double>& new_objective_function, vector<double>& old_objective_function);
  bool remove_variable(string var_name);
  bool remove_variable(int var_col);
  bool swap_col(int pos1, int pos2);
  bool swap_row(int pos1, int pos2);
  
  // displayers
  void print(const bool show_comparator = false) const;
  void print(const string msg, const bool show_comparator = false) const;
private: 
  int find_pivot_column() const;
  int find_pivot_row(int pivot_col) const;
  void pivot_on(int pivot_col, int pivot_row);
  vector<vector<double>> matrix;
  vector<string> variables;
  vector<comparator> comparators;
};

#endif

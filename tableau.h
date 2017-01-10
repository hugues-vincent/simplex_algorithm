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

  // getters
  int get_base_row_positon(string var_name) const;
  int get_var_position(string var_name) const;
  int get_nb_var() const;
  int get_nb_row() const;
  vector<vector<double>> get_matrix() const;
  vector<string> get_variables() const;

  // modfiers
  int add_variable(string var_name);
  bool add_row(vector<double> row, comparator comp = EQUAL);
  bool add_row(vector<double> row, vector<string> var_names, comparator comp = EQUAL);
  bool add_slacks();
  bool remove_variable(string var_name);
  bool swap_col(int pos1, int pos2);
  bool swap_row(int pos1, int pos2);
  
  // displayers
  void print(const bool show_comparator = false) const;
  void print(const string msg, const bool show_comparator = false) const;
private: 
  vector<vector<double>> matrix;
  vector<string> variables;
  vector<comparator> comparators;

};

#endif

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>

#include "tableau.h"

using namespace std;

class parser
{
public:
  static bool parse_file(const string& file_path, tableau& table);
private: 
  static vector<vector<string>> matrix, col_names;
  static vector<vector<double>> col_variables;
  static vector<comparator> comparators;
  static int nb_var;
  static bool reader(const string& file_path, tableau& tab);
  static bool fills_vectors_from_line(string& line, tableau& tab);
  static bool fills_tableau_from_vectors(tableau& tab);
  parser();
};

#endif

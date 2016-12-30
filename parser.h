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
  static vector<vector<string>> matrix;
  static vector<string> col_names;
  static vector<double> col_variables;
private: 
  static bool reader(const string& file_path);
  static bool fills_vectors_from_file(string& line);
  static bool fills_tableau_from_vectors();
  static bool standard_form(tableau& table);
  parser();
};

#endif

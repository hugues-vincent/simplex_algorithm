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
private: 
  static bool reader(const string& file_path);
  static bool standard_form(tableau& table);
  parser();
};

#endif

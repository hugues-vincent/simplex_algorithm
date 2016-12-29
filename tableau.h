#ifndef TABLEAU_H
#define TABLEAU_H

#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

class tableau
{
public:
  // Constructeurs
  tableau();
  // Tableau(double x, double y);

  //Accesseurs et mutateurs
  // void setY(double y);
  // void setY(double y);
  // double getX() const;
  // double getY() const;

  void add_variable(string var_name);
  void add_row(vector<double> row);
  void add_row(vector<double> row, set<string> var_names);
  // Autres méthodes
  // double distance(const Tableau &P) const;

  // void saisir();
  void print() const;
  void print(const string msg) const;

private: 
  vector<vector<double>> matrix;
  vector<string> variables;

};

#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <numeric>

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
void binary(tableau& tab)
{
	double z, z_0, z_1;
	vector<double> thetas, thetas_0, thetas_1;
	vector<bool> result;
	
	for(int i=0; i<tab.get_nb_var(); i++)
	{

	}
}

int main(int argc, char *argv[])
{
	tableau t, t1;
	tableau& table=t, rt=t1;
	
	if(argc>1)
	{
		if(!parser::parse_file(argv[1], rt)) cout << "file isn't readable\n"; 
	}
	else cout << "Can't reach the file\n";
	

	return 0; 
}



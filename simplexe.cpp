#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

// containers
#include <vector>

// own classes
#include "tableau.h"
#include "parser.h"

using namespace std;

int main(int argc, char *argv[])
{
	tableau t;
	tableau& table=t;
	vector<double> v{3, 9};
	table.add_row(vector<double> {3, 9}, set<string> {"x", "y"});
	table.add_row(vector<double> {1, 19});
	table.add_row(vector<double> {1, 19, 22});
	table.add_row(vector<double> {2, 1, 99}, set<string> {"z"});
	table.add_variable("lol");
	table.print();
	
	if(argc>1){
		// ifstream& file = ifstream(argv[1]);
		if(parser::parse_file(argv[1], table))
		{
			table.print();
		}	
		else cout << "file isn't readable\n"; 
	}
	else cout << "Can't reach the file\n";
	return 0; 
}



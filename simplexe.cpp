#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

// containers
#include <tuple>
#include <map>
#include <vector>

// own classes
#include "tableau.h"

using namespace std;

bool readFile(ifstream file, tableau& table)
{
	bool file_readable=true;
	string line;
	while(file_readable && getline(file, line))	
	{
	    vector<string> words;
		cout << "";
	    regex rgx("\\w+");
	    for(sregex_iterator it(line.begin(), line.end(), rgx), it_end; it != it_end; ++it)
	    {
	    	words.push_back((*it)[0]);
	        cout << (*it)[0] << "\n";
	    }
	    cout << "\n";
	    // for each(string word in words)
	    // {
	    // 	cout << word << "\n";
	    // }
	}
	return true;
}


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
		if(readFile(ifstream(argv[1]), table))
		{
			table.print();
		}	
		else cout << "file isn't readable\n"; 
	}
	else cout << "No file used with the command\n";
	// 	string line, word;
	// 	// vector<vector<double>> 
	// 	readFile(ifstream(argv[1]));

		// while (getline(infile, line)){
		// 	if(!regex_match(line, regex("^MaxB.*")))
		// 	{
		// 		regex reg("\\w+");
		// 		regex_iterator<string::iterator> 
		// 			it(line.begin(), line.end(), reg),
		// 			it_end;

		// 	    for(it, it_end; it != it_end; ++it )
  //       			cout << (*it)[0] << "\n";

				// istringstream iss(line);
			 //    while (iss >> word)
			 //    {
				// 	cout << word << " -> ";

				// 	if(regex_match(word, regex("\\+?[1-9]+$")))
				// 	{
				// 		cout << "digit";
				// 	}
					 
				// 	else if(regex_match(word, regex("^[a-z]+$")))
				// 	{
				// 		cout << "alpha";
				// 		// simplex_var.insert(new map_variable(word, 1));
				// 	}
	   //      		cout<< endl;
			    	
			 //    }
				
			// }
			// else cout << " pas bon"<<endl; 
	        // cout<< endl;
		// }
	// set<string>::const_iterator
	// 	sit (simplex_var.begin()),
	// 	send(simplex_var.end()); 
	// for(;sit!=send;++sit) cout << *sit << ' '; 
	// cout << endl;
	// }
	return 0; 
}



# simplex algorithm

Simplex.cpp is the entry point of the project (i.e where is defined the main() function). This is where are defined :
- lp_full: simplex algorithm with Real number solution; 
- lp_integer: simplex algorithm with Integer solution;
- lp_binary : simplex algorithm with binary solution.

Tableau.cpp is a class that wrap the matrix concept for linear manipulations of equations

Parser.cpp is function that parse linear programming problem input, for instance:
max 3x + 4y
3x -y <= 12
3x + 11y  <= 66

# run the code

1. write a linear programming problm inside a file like lp.txt. You must follow the same pattern as above. 
2. compile the code with: ```g++ -std=c++11``` 
3. run it with: ```./a.out lp.txt```

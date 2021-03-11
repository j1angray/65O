# 65O

# PROJECT OVERVIEW
The project is to help the local police department with their installation of security cameras at traffic intersections. You will solve a particular kind of optimization problem, called the Vertex Cover problem, in this context. The idea is for the police to be able to minimize the number of cameras they need to install, and still be as e↵ective as possible with their monitoring.


# MAIN MISSIONS

A1
* Take as input a series of commands that describe streets.
* Use that input to construct a particular kind of undirected graph.

A2
* One of the kinds of input contributes towards the specification of an undirected graph.
* Determine a shortest-path from one vertex to another in the current graph. Algorithms like Breadth-First Search or Bellman-Ford can be used.

A3
* Modify the output format of your Python script from A1 to match the input format of your C++ program from A2.
* Modify your C++ program from A2 to output the input graph on standard output.
* Write a program in C++ to generate random input for your Python script.
* Write a driver program in C++ that uses Inter-Process Communication (IPC) to link theoutput of the random input generator to the input of the Python script, and the output of the Python script to the input of the C++ program from A2.

A4
* Implement the reduction and use minisat as a library to solve the minimum VERTEX COVER problem for the graphs that are input to your program
* Create a polynomial reduction of the decision version of VERTEX COVER to CNF-SAT.
* As soon as you get an input graph via the ’V’ and ’E’ specification you should compute a minimum-sized Vertex Cover, and immediately output it.

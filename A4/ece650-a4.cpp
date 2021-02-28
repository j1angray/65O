#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"
#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <list>


using namespace std;


bool check_edge(int v, int srce, int dest)
{
    if (srce >= 0 && dest >= 0 && srce < v && dest < v && srce != dest)
    {
        return true;
    }
    else if (srce == dest)
    {
        cout << "Error: Can't generate edge from the vertice itself. " << endl;
        return false;
    }
    else
    {
        cout << "Error: Exceeded value. No such vertice. " << endl;
        return false;
    }
}



vector< pair<int,int> > check_pairs(int v, string str)
{
    std::vector< std::pair<int,int> > pairs;
    pair<int, int> edge;
    regex  edges_pattern("\\d+,\\d+");
    smatch results;
    regex  vertice_pattern("(\\d+),(\\d+)");
    smatch result;
    string::const_iterator iterStart = str.begin();
    string::const_iterator iterEnd = str.end();
    string edges;
    while (regex_search(iterStart, iterEnd, results, edges_pattern))
    {
        edges = results[0];
        if (regex_match(edges, result, vertice_pattern))
        {
            int srce = stoi(result[1].str());
            int dest = stoi(result[2].str());
            if (check_edge(v, srce, dest))
            {
                edge.first = srce;
                edge.second = dest;
                pairs.push_back(edge);
            }
            else
            {
                break;
            }
        }
        iterStart = results[0].second;
    }
    return pairs;
}



void Clause_1(Minisat::Solver& solver, int v, int k) //At least one vertex is the ith vertex in the k-size vertex cover
{
    for (int i = 0; i < k; i++) { 
        Minisat::vec<Minisat::Lit> literals;
        for (int n = 0; n < v; n++) {
            Minisat::Var var_ni = solver.newVar();
            var_ni = n * k + i; // the X(n,i)'s location in the n × k atomic propositions
            literals.push(Minisat::mkLit(var_ni));
        }
        solver.addClause(literals);
    }
}



void Clause_2(Minisat::Solver& solver, int v, int k) //Vertex m cannot appear both in positions p and q (p < q) of the k-size vertex cover
{
    for (int m = 0; m < v; m++) {   
            for (int q = 0; q < k; q++) {
                for (int p = 0; p < q; p++) {
                    Minisat::Var var_mp = solver.newVar();
                    Minisat::Var var_mq = solver.newVar();
                    var_mp = m * k + p;
                    var_mq = m * k + q;
                    solver.addClause( ~Minisat::mkLit(var_mp), ~Minisat::mkLit(var_mq));  
                }   
            }        
        }
}



void Clause_3(Minisat::Solver& solver, int v, int k) //Only Vertex p or only Vertex q (p < q) can appear in the mth position of the k-size vertex cover
{
    for (int m = 0; m < k; m++) {   
        for (int q = 0; q < v; q++) {
            for (int p = 0; p < q; p++) {
                Minisat::Var var_mp = solver.newVar();
                Minisat::Var var_mq = solver.newVar();
                var_mp = p * k + m;
                var_mq = q * k + m; 
                solver.addClause( ~Minisat::mkLit(var_mp), ~Minisat::mkLit(var_mq));  
            }   
        }        
    }

}



void Clause_4(Minisat::Solver& solver, int vertices, int k, vector< pair<int,int> > edgepairs) //Every edge <i,j> is incident to at least one vertex in the k-size vertex cover
{
    for ( auto& e : edgepairs) {
        Minisat::vec<Minisat::Lit> literals;
        for (int m = 0; m < k; m++) {
            int ei = e.first;
            int ej = e.second;
            Minisat::Var var_ei = solver.newVar();
            Minisat::Var var_ej = solver.newVar();
            var_ei = ei * k + m;
            var_ej = ej * k + m; 
            literals.push(Minisat::mkLit(var_ei));
            literals.push(Minisat::mkLit(var_ej));
            }
        solver.addClause(literals);
        }
}



bool build_sat(Minisat::Solver& solver, int v, int k, vector< pair<int,int> > edgepairs) 
{
    for (int row = 0; row < v; row++) {
        for (int col = 0; col < k; col++) {
            auto var = solver.newVar(); //initiate variables in solver
        }
    }

    Clause_1(solver, v, k);
    Clause_2(solver, v, k);   
    Clause_3(solver, v, k);
    Clause_4(solver, v, k, edgepairs);

    auto sat = solver.solve();
    if (sat) 
        return sat;
    else 
        return false;
}



vector<int> build_solution(Minisat::Solver& solver, int v, int k, vector< pair<int,int> > edgepairs)
{ 

    std::vector<int> result;
    
    for (int row = 0; row < v; row++) {
        for (int col = 0; col < k; col++) {
            Minisat::Var loc = solver.newVar();
            loc = row * k + col;
            if (solver.modelValue(loc) == Minisat::l_True) {
                result.push_back(row); // row id = vertex id
            }
        }      
    }
    
    std::sort(result.begin(), result.end());

    for (int i = 0; i < result.size(); ++i) {
        cout << result[i] << " ";
    }
    cout << "\n";

    return result;

}   



void get_cmd()
{
    int v;
    string input_edges;
    char cmd;

    vector< pair<int,int> > edges;

    Minisat::Solver s;

    while(cin >> cmd)
    {
        if (cmd == 'V') {
            int a;
            cin >> a;
            if (a > 0){
                v = a;
            }
            else{
                cout <<"Error: Invalid value. " << endl;
            }
        }
        if (cmd == 'E') {
            cin >> input_edges;

            if (input_edges == "{}")
                continue;

            vector< pair<int,int> > edges;
            edges = check_pairs(v, input_edges);
            int k;

            for (k = 1; k <= v;k++){
                Minisat::Solver s;
                if (build_sat(s, v, k, edges) == true){
                    build_solution(s, v, k, edges);
                    break;
                }
            }
            if (k > v) {
              cout << "Error: No SAT result!" << endl;
            }

        }

    }
}


int main()
{
    get_cmd();
}


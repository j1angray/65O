#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <list>

using namespace std;


bool check_edges(int v, int srce, int dest)
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


void add_edges(int srce, int dest, vector<int> adj_list[])
{
    adj_list[srce].push_back(dest);
    adj_list[dest].push_back(srce);
}


void check_edges(int v, string str, vector<int> adj_list[])
{
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
            if (check_edges(v, srce, dest))
            {
                add_edges(srce, dest, adj_list);
            }
            else
            {
                break;
            }
        }
        iterStart = results[0].second;
    }
    
}

    
bool BreadthFirst_search(int srce, int dest, int v, vector<int> adj_list[], int pred[])
{
    list<int> vs_list; //vertices with its adjacency list 
    bool vs_visited[v]; //whether the vertex of each adjacency list  is visited or not 
    
    for (int i = 0; i < v; i++)
    {
        vs_visited[i] = false;
        pred[i] = -1;    
    }
    
    vs_visited[srce] = true;
    vs_list.push_back(srce);
    
    while (!vs_list.empty())
    {
        
        int u = vs_list.front();
        vs_list.pop_front();
        
        for (int i = 0; i < adj_list[u].size(); i++)
        {
            if (vs_visited[adj_list[u][i]] == false)
            {
                vs_visited[adj_list[u][i]] = true;
                pred[adj_list[u][i]] = u;
                vs_list.push_back(adj_list[u][i]);
                if (adj_list[u][i] == dest) //find a path from (vertex)source to (vertex)destination
                    return true;
            }
        }

    }
    return false;
}


void output_ShortestPath(int srce, int dest, int v, vector<int> adj_list[])
{
    int pred[v];
    vector<int> path; //if there is a path then push back each (vertice)stop
    
    if (BreadthFirst_search(srce, dest, v, adj_list, pred) == false)
    {
        cout << "Error: No such path between the 2 vertices. " << endl;
        return;
    }
    
    int st = dest;
    path.push_back(st);

    while (pred[st] != -1)
    {
        path.push_back(pred[st]);
        st = pred[st];
    }
    
    for (int i = int(path.size() - 1); i > 0; i--)
    {
        cout << path[i] << "-";
    }
    cout << path[0] << endl;
}



void get_cmd(vector<int> *&adj)
{
    int v;
    int src;
    int dst;
    string input_edges;
    char cmd;
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
            adj = new vector<int>[v];
            check_edges(v, input_edges, adj);
        }
        if (cmd == 's') {
            cin >> src >> dst;
            if (src == dst ){
                cout <<"Error: Can't generate path from the vertice itself." << endl;
            }
            else if (src < v && dst < v){
                output_ShortestPath(src, dst, v, adj);
            }
            else{
                cout <<"Error: No such vertice." << endl;
            }
        }      
    }
}


int main()
{
    vector<int> *adj;
    get_cmd(adj);
}
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <random>
#include <unistd.h>
#include <cmath>
#include <algorithm>


using namespace std;



int urandom_gen() //REF: https://stackoverflow.com/questions/35726331/c-extracting-random-numbers-from-dev-urandom
{
    int i = 0;
    unsigned int random_int = 0; //Declare value to store data
    size_t size = sizeof(random_int); //Declare size of data
    ifstream urandom("/dev/urandom", ios::in|ios::binary); //Open stream
    if(urandom) //Check if stream is open
    {
        urandom.read(reinterpret_cast<char*>(&random_int), size); //Read from urandom
        if(urandom)
        {
            //cout << "Random data from Urandom: " << random_int << endl;
            i = random_int;
        }
        else
        {
            cerr << "Error: Failed to read from /dev/urandom" << endl;
        }
        urandom.close(); //close stream
    }
    else //Open failed
    {
        cerr << "Error: Failed to open /dev/urandom" << endl;
        return 0;
    }
    return abs(i);
}



int int_rgen(int min, int max) // generate a random int in a closed interval
{
    int i = urandom_gen();
    int rand_int = min + abs(i % ( max - min + 1));
    return rand_int;
}



string string_rgen()  //generate a random string
{
    int i = urandom_gen();
    string rand_string;
    int len = i%10 + 1;                  //random value of string length

    for(int i = 1; i <= len; i++)
    {
        int x;                        //ASCII codes of a character
        x = 'a' + i % ('z' - 'a' + 1);
        char c = (char)x;
        rand_string += c;
    }
    
    return rand_string;
}



vector<int> seg_func(pair<int,int> vs,pair<int,int> ve)
{
    vector<int> seg_para;
    int a = vs.second - ve.second;
    int b = ve.first - vs.first;
    int c = vs.first * ve.second - ve.first * vs.second;
    seg_para.push_back(a);
    seg_para.push_back(b);
    seg_para.push_back(c);
    return seg_para;
}



bool seg_check(pair<int,int> v1, pair<int,int> v2, pair<int,int> v3, pair<int,int> v4){
    vector<int> sp1 = seg_func(v1,v2);
    vector<int> sp2 = seg_func(v3,v4);

    float d = float(sp1[1] * sp2[0] - sp1[0] * sp2[1]);
    
    int seg1_xlower = min(v1.first,v2.first);
    int seg1_xupper = max(v1.first,v2.first);
    int seg1_ylower = min(v1.second,v2.second);
    int seg1_yupper = max(v1.second,v2.second);
    
    int seg2_xlower = min(v3.first,v4.first);
    int seg2_xupper = max(v3.first,v4.first);
    int seg2_ylower = min(v3.second,v4.second);
    int seg2_yupper = max(v3.second,v4.second);
    
    if (d==0) //seg(v1,v2) is parallel to seg(v3,v4), check overlap
    {
        if ((sp1[1]==0) && (sp2[1]==0) && (v1.first==v3.first)) //parallel to the y-axis
        {
            if ((seg1_yupper < seg2_ylower) || (seg2_yupper < seg1_ylower))
            {
                //cout << "Valid"<< endl;
                return true;
            }
            else //overlap
            {
                //cout << "OVERLAP"<< endl;
                return false;
            }
        }
        if ((v1.second + v1.first * sp1[0] / sp1[1]) == (v3.second + v3.first * sp2[0] / sp2[1])) //unparallel to the y-axis
        {
            if ( ((seg2_xupper < seg1_xlower )&&(seg2_yupper < seg1_ylower)) || ((seg1_xupper < seg2_xlower)&&(seg1_yupper < seg2_ylower)) )
            {
                //cout << "Valid"<< endl;
                return true;
            }
            else //overlap
            {
                //cout << "OVERLAP"<< endl;
                return false;
            }
        }
    }
    else if(v2!=v3)//seg(v1,v2) is unparallel to seg(v3,v4), check intersect
    {
        float x = float(float(sp2[1] * sp1[2] - sp1[1] * sp2[2]) / d);
        float y = float(float(sp1[0] * sp2[2] - sp2[0] * sp1[2]) / d);
        //cout << "x:"<< x<<";  y:"<<y<< endl;
        if ((seg1_xlower <= x && x <= seg1_xupper) && (seg2_xlower <= x && x <= seg2_xupper) && (seg1_ylower <= y && y <= seg1_yupper) && (seg2_ylower <= y && y <= seg2_yupper)) //intersect
        {
            //cout << "INTERSECT"<< endl;
            return false;
        }
        else
        {
            //cout << "VALID"<< endl;
            return true;
        }
    }
    return true;
}



bool strname_check(vector<string> strs, string s)
{
    vector<string>::iterator result = find( strs.begin( ), strs.end( ), s);
    if ( result == strs.end( ) ) //Not found, no repeat element
    {
        //cout << "Not Found" << endl;
        return true;
    }
    else //Found repeat element
    {
        //cout << "Repeat" << endl;
        return false;
    }
}



bool strvertices_check(vector<pair<int,int> > vertices)
{

    for (int i=0; i < vertices.size(); i++)
    {
        for (int j=i+1; j+1 < vertices.size(); j++)
        {
            if( (vertices[i].first == vertices[j].first) && (vertices[i].second == vertices[j].second) )
            {
                //cout << "Repeat vertex" << endl;
                return false;
            }
            if(seg_check(vertices[i], vertices[i+1], vertices[j], vertices[j+1]) == false)
            {
                //cout << "Wrong segment" <<endl;
                return false;
            }
        }
    }
    return true;
}



vector<pair<int,int> > vertices_gen(int n_arg, int c_arg) {
    pair<int,int> vertex;
    vector<pair<int,int> > vertices;
    
    int seg_num = int_rgen(1, n_arg);

    int times=0;
    
    do
    {
        vertices.clear();
        for(int i = 0; i < seg_num + 1; i++) // vertex_num = seg_num + 1
        {
            vertex.first = int_rgen(-c_arg, c_arg);
            vertex.second = int_rgen(-c_arg, c_arg);
            vertices.push_back(vertex);
        }
        times++;
        if (times > 25) {
            cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
        }
    }while (strvertices_check(vertices) == false);

    return vertices;
}



vector<pair<string,vector<pair<int,int> > > > graph_gen(int s_arg,int n_arg, int c_arg){
    int str_num = int_rgen(2, s_arg);
    
    vector<string> strs_names;
    vector<vector<pair<int,int> > > strs_vertices;
    vector<pair<string,vector<pair<int,int> > > > graph;
    
    string str_name;
    vector<pair<int,int> > str_vertices;
    

    for(int i=0; i < str_num; i++)
    {
        str_name = string_rgen();
        while (strname_check(strs_names,str_name) == false)
        {
            str_name = string_rgen();
        }
        strs_names.push_back(str_name);
    }


    for (int n = 0; n < strs_names.size(); n++ )
    {
        str_vertices = vertices_gen(n_arg,c_arg);
        strs_vertices.push_back(str_vertices);
    }

    for (int a=0; a < strs_names.size(); a++) 
    {
        pair<string,vector<pair<int,int> > > street;
        street.first=strs_names[a];
        street.second=strs_vertices[a];
        graph.push_back(street);
    }
    
    return graph;

}



void output_a_cmd(vector<pair<string,vector<pair<int,int> > > > graph)
{
    string cmd;
    for (int i = 0; i < graph.size(); i++) {
        pair<string,vector<pair<int,int> > > streettmp = graph[i];
        cmd = cmd + "a \"" + streettmp.first + "\" ";
        vector<pair<int,int> > strs_vertices = streettmp.second;
        for (int v = 0; v < strs_vertices.size(); v++) {
            cmd = cmd + "(" + std::to_string(strs_vertices[v].first) + "," + std::to_string(strs_vertices[v].second) + ")";
        }
        cmd += "\n";
    }
    cmd += "g";
    cout << cmd << endl;
}



void output_r_cmd(vector<pair<string,vector<pair<int,int> > > > graph)
{
    string cmd;
    for (int i = 0; i < graph.size(); i++) {
        pair<string,vector<pair<int,int> > > streettmp = graph[i];
        if (i < graph.size()-1)
            cmd = cmd + "r \"" + streettmp.first + "\"\n";
        else
            cmd = cmd + "r \"" + streettmp.first + "\"";

    }
    cout << cmd << endl;
}






int main(int argc, char **argv){
 
    int s_arg; //number of streets
    int n_arg; //number of line segments
    int l_arg; //wait a random number
    int c_arg; //coordinate range
 
    s_arg = 10; //default value of the number of streets
    n_arg = 5; //default value of the number of line segments
    l_arg = 5; //default value of the random number to wait
    c_arg = 20; //default value of the coordinate range
 
    int o;
 
    opterr = 0; //prevent the error message
 
    while ((o = getopt(argc, argv, "s:n:c:l:?")) != -1)
    {
        switch (o)
        {
            case 's':
                s_arg = atoi(optarg);
                //cout<< "opt is s, oprarg is: " << optarg << endl;
                if(s_arg < 2) {
                    std::cerr << "Error: option -s takes an arg that is NOT an integer ≥ 2" << std::endl;
                    exit(1);
                }
                break;
 
            case 'n':
                n_arg = atoi(optarg);
                //cout<< "opt is n, oprarg is: " << optarg << endl;
                if(n_arg < 1) {
                    cerr << "Error: option -n takes an arg that is NOT an integer ≥ 1" << endl;
                    exit(1);
                }
                break;
 
            case 'l':
                l_arg = atoi(optarg);
                //cout<< "opt is l, oprarg is: " << optarg << endl;
                if(l_arg < 5) {
                    cerr << "Error: option -n takes an arg that is NOT an integer ≥ 5" << endl;
                    exit(1);
                }
                break;
 
            case 'c':
                c_arg = atoi(optarg);
                //cout<< "opt is c, oprarg is: " << optarg << endl;
                if(c_arg < 1) {
                    cerr << "Error: option -n takes an arg that is NOT an integer ≥ 1" << endl;
                    exit(1);
                }
                break;
 
            case '?':
                cerr << "Error: wrong option " << endl;
                exit(1);
 
            default:
                return 0;
 
        }
    }
    

        while(true)
        {
            vector<pair<string,vector<pair<int,int> > > > graph = graph_gen(s_arg,n_arg,c_arg);
            output_a_cmd(graph);
            sleep(int_rgen(5,l_arg));
            output_r_cmd(graph);
            graph.clear();
        }  
    
        return 0;
     
 }
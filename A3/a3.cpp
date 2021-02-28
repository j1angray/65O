#include <stdio.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <vector>

using namespace std;

int getln(void) 
{
    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
            std::cout << line << std::endl;
    }
    return 0;
}


int rgen(int argc, char **argv) 
{
    argv[0] = (char*) "./rgen";
    execv("./rgen", argv);
    std::cerr << "Error: fail to execute rgen" << std::endl;
    return 0;
}

int exec_a1(void)
{
    char* arg_a1[3] = {"python","ece650-a1.py", NULL};
    execvp("python", arg_a1);
    std::cerr << "Error: fail to execute ece650-a1.py" << std::endl;
    return 0;
}


int exec_a2(void)
{
    char* arg_a2[2] = {"ece650-a2.cpp", NULL};
    execv("ece650-a2", arg_a2);
    std::cerr << "Error: fail to execute ece650-a2.cpp" << std::endl;
    return 0;
}



int main(int argc, char **argv)
{
    std::vector<pid_t> kids;

    int rgen_a1[2];
    pipe(rgen_a1);

    int a1_a2[2];
    pipe(a1_a2);


    pid_t child_pid;
    child_pid = fork();

    if (child_pid == 0) {
    
        dup2(rgen_a1[1], STDOUT_FILENO);
        //cout << "dup" << endl;        
        close(rgen_a1[0]);
        close(rgen_a1[1]);
        //cout << "close" << endl;
        return rgen(argc, argv);
    }
    
    else if (child_pid < 0) {
        std::cerr << "Error: fail to fork rgen" << std::endl;
        return 1;  
    }
  

    kids.push_back(child_pid);


    child_pid = fork();

    if (child_pid == 0) {
        //cout << "into rgen_a1" << endl;
        dup2(rgen_a1[0], STDIN_FILENO);
        close(rgen_a1[0]);
        close(rgen_a1[1]);

        dup2(a1_a2[1], STDOUT_FILENO);
        //cout << "dup to a2" << endl;
        close(a1_a2[0]);
        close(a1_a2[1]);
        //cout << "close" << endl;       
        return exec_a1();
    }
    else if (child_pid < 0) {
        std::cerr << "Error: fail to fork a1" << std::endl;
        return 1;
    }



    //dup2(a1_a2[0], STDIN_FILENO);

    //close(a1_a2[0]);
    //close(a1_a2[1]);

    //getln();
    

    kids.push_back(child_pid);


    child_pid = fork();

    if (child_pid == 0) {
        //cout << "into a1_a2" << endl;
        dup2(a1_a2[0], STDIN_FILENO);
        close(a1_a2[1]);
        close(a1_a2[0]);
        return exec_a2();
    } 
    else if (child_pid < 0) {
        std::cerr << "Error: fail to fork a2" << std::endl;
        return 1;  
    }


    dup2(a1_a2[1], STDOUT_FILENO);
    close(a1_a2[0]);
    close(a1_a2[1]);

    getln();

    for (pid_t k : kids) { 
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }


    return 0;

}


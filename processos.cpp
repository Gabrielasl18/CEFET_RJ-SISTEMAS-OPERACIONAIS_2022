#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[], char *envp[]){
    cout << "processo pai iniciou\n";

    pid_t  pid = fork();

    if (pid==0){
        execve("/usr/bin/cal",argv,envp);
    }
    else if(pid > 0){
        //cout << "Eu sou o pai e o pid do meu filho e: " << (int)pid << "\n";
        while(1);
    }
    else{
        cout << "Erro ao criar processo\n";
    }

    wait(NULL); //o processo foi suspenso

    if (pid>0){
        cout << "Pai terminando\n";
    }
    else{
        cout << "Filho terminando\n";
    }
    return 0;
}

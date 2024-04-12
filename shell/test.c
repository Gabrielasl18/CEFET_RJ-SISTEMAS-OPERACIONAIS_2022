
//processo para cada novo comando
//usar execv
//analisar a linha de entrada em partes constituintes, você pode usar strsep()
//em qualquer modo, se você atingir o marcador de fim de arquivo (EOF), deverá chamar exit(0) 
//Para ler linhas de entrada, você deve usar getline()
//modo interativo = usar stdin
//shell executado em loop while

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

// Função para imprimir a mensagem de erro
void print_error() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

int main(int argc, char *argv[]) {

    char command[MAX_COMMAND_LENGTH];
    char error_message[30] = "An error has occurred\n";
    
    if (argc > 1) {
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        pid_t pid_filho = fork();
        char command[10], options[10], path[512];
        
        if(pid_filho < 0) {
            print_error();
            return 1;
        }
        
        if(pid_filho == 0) {
            fp = fopen(argv[1], "r");
            if (fp == NULL) {
                print_error();
                exit(1);
            }
            while (getline(&line, &len, fp) != -1) {
                printf("%s", line);
            }
            sscanf(line, "%s %s %s", command, options, path);
        
            char *childArgs[4] = {command, options, path, NULL};
            execvp(childArgs[0], childArgs);
            fclose(fp);
        } else {
            pid_filho - wait(NULL);
            printf("Eu sou o processo pai (PID %d). \nMEu processo filhot em PID %d. \n", getpid(), pid_filho);
        }
        exit(0);
    }

    while(1) {
        printf("wish> ");
        fflush(stdout);

        // Ler a entrada do usuário
        if (fgets(command, sizeof(command), stdin) == NULL) {
            // Se não houver entrada, sair do loop
            break;
        }

        // Remover o caractere de nova linha
        command[strcspn(command, "\n")] = '\0';

        // Criar um novo processo filho
        pid_t pid_filho = fork();

        if (pid_filho < 0) {
            print_error();
            exit(EXIT_FAILURE);
        }

        if (pid_filho == 0) {    
            // Processo filho
            // Executar o comando
            execlp(command, command, NULL);

            // Se execlp retornar, algo deu errado
            exit(EXIT_FAILURE);
        } else {
            // Processo pai
            // Esperar pelo término do processo filho
            wait(NULL);
        }
    }

    return 0;
}

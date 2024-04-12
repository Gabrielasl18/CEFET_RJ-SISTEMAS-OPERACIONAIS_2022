#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_LEN_INPUT 1024
#define MAX_ARGS_SUPPORTED 64
#define DELIMIT_CHARS_USED " \t\r\n\a"

char *list_of_internal_cmds[] = {"leave", "switch", "track"};

void print_error_msg() {
    char msg_error[] = "An unexpected issue has occurred\n";
    write(STDERR_FILENO, msg_error, strlen(msg_error));
}

int check_internal_cmd(char *cmd) {
    int num_internal_cmds = sizeof(list_of_internal_cmds) / sizeof(list_of_internal_cmds[0]);
    for (int i = 0; i < num_internal_cmds; i++) {
        if (strcmp(cmd, list_of_internal_cmds[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void perform_cmd_execution(char **args, char *input_file, char *output_file, int is_background) {
    int process_id = fork();
    if (process_id == 0) {
        if (input_file) {
            int file_desc = open(input_file, O_RDONLY);
            if (file_desc == -1) {
                print_error_msg();
                exit(EXIT_FAILURE);
            }
            dup2(file_desc, STDIN_FILENO);
            close(file_desc);
        }
        if (output_file) {
            int file_desc = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (file_desc == -1) {
                print_error_msg();
                exit(EXIT_FAILURE);
            }
            dup2(file_desc, STDOUT_FILENO);
            dup2(file_desc, STDERR_FILENO); 
            close(file_desc);
        }
        if (execvp(args[0], args) == -1) {
            print_error_msg();
            exit(EXIT_FAILURE);
        }
    } else if (process_id > 0) {
        if (!is_background) {
            wait(NULL);
        }
    } else {
        print_error_msg();
    }
}

void process_command(char **args) {
    if (strcmp(args[0], "leave") == 0) {
        if (args[1] != NULL) {
            print_error_msg();
        } else {
            exit(EXIT_SUCCESS);
        }
    } else if (strcmp(args[0], "switch") == 0) {
        if (args[1] == NULL) {
            print_error_msg();
        } else {
            if (chdir(args[1]) != 0) {
                print_error_msg();
            }
        }
    } else if (strcmp(args[0], "track") == 0) {
        if (args[1] == NULL) {
            print_error_msg();
        } else {
            char *new_path = args[1];
            if (setenv("TRACKER", new_path, 1) != 0) {
                print_error_msg();
            }
        }
    } else {
        perform_cmd_execution(args, NULL, NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    FILE *input_stream = stdin;
    if (argc > 2) {
        print_error_msg();
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        input_stream = fopen(argv[1], "r");
        if (input_stream == NULL) {
            print_error_msg();
            exit(EXIT_FAILURE);
        }
    }

    char input[MAX_LEN_INPUT];
    while (1) {
        if (input_stream == stdin) {
            printf("invoke> ");
        }

        if (fgets(input, MAX_LEN_INPUT, input_stream) == NULL) {
            if (feof(input_stream)) {
                exit(EXIT_SUCCESS);
            } else {
                print_error_msg();
                exit(EXIT_FAILURE);
            }
        }

        char *args[MAX_ARGS_SUPPORTED];
        char *input_file = NULL;
        char *output_file = NULL;
        int is_background = 0;

        char *token = strtok(input, DELIMIT_CHARS_USED);
        int num_args = 0;
        while (token != NULL) {
            if (strcmp(token, "<") == 0) {
                token = strtok(NULL, DELIMIT_CHARS_USED);
                if (token == NULL) {
                    print_error_msg();
                    break;
                } else {
                    input_file = token;
                }
                token = strtok(NULL, DELIMIT_CHARS_USED);
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, DELIMIT_CHARS_USED);
                if (token == NULL) { 
                    print_error_msg();
                    break;
                } else {
                    output_file = token;
                }
                token = strtok(NULL, DELIMIT_CHARS_USED); 
            } else if (strcmp(token, "&") == 0) {
                is_background = 1;
                break;  
            } else {
                args[num_args++] = token;
            }
            token = strtok(NULL, DELIMIT_CHARS_USED);
        }

        args[num_args] = NULL;

        if (num_args > 0) {
            if (check_internal_cmd(args[0])) {
                process_command(args);
            } else {
                perform_cmd_execution(args, input_file, output_file, is_background);
            }
        }
    }

    return 0;
}

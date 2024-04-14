#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define INITIAL_VECTOR_CAPACITY 10

typedef struct {
    char** data;
    int capacity;
    int size;
} CustomVector;

CustomVector create_custom_vector() {
    CustomVector v;
    v.data = (char**) malloc(INITIAL_VECTOR_CAPACITY * sizeof(char*));
    v.capacity = INITIAL_VECTOR_CAPACITY;
    v.size = 0;
    return v;
}

void custom_push_back(CustomVector* v, char* val) {
    if (v->capacity == v->size) {
        char** new_data = (char**) malloc( (v->capacity << 1) * sizeof(char*));
        int n = v->size;
        for (int i = 0; i < n; i++) new_data[i] = v->data[i];
        free(v->data);
        v->data = new_data;
        v->capacity <<= 1;
    }
    v->data[v->size++] = val;
}

void custom_pop_back(CustomVector* v) {
    if (v->size > 0) v->size--;
}

char* custom_get(CustomVector* v, int index) {
    if (index < 0 || index >= v->size) return NULL;
    return v->data[index];
}

int custom_get_size(CustomVector* v) {
    return v->size;
}

int custom_search_key(CustomVector* v, char* key) {
    int n = v->size;
    for (int i = 0; i < n; i++) {
        if (strcmp(key, v->data[i]) == 0) return i;
    }
    return -1;
}

void custom_destroy(CustomVector* v) {
    free(v->data);
}

CustomVector custom_PATH;
int custom_is_delimiter(char c);

CustomVector custom_parse(char* line) {
    CustomVector ans = create_custom_vector();
    int n = strlen(line);
    char* s = NULL;
    int start = -1;
    
    for (int i = 0; i < n; i++) {

        if (line[i] == '>') {
            custom_push_back(&ans, ">");
            continue;
        }

        if (line[i] == '&') {
            custom_push_back(&ans, "&");
            continue;
        }

        if (!custom_is_delimiter(line[i])) {
            if (i == 0 || custom_is_delimiter(line[i-1])) start = i;
            if (i == n-1 || custom_is_delimiter(line[i+1])) {
                s = strndup(line+start, i-start+1);
                custom_push_back(&ans, s);
            }
        }
    }
    return ans;
}


int custom_is_same_file(int fd1, int fd2) {
    struct stat stat1, stat2;
    if(fstat(fd1, &stat1) < 0) return -1;
    if(fstat(fd2, &stat2) < 0) return -1;
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

int custom_is_delimiter(char c) {
    return (c == ' ') || (c == '\t') || (c == '>') || (c == '&');
}

int custom_is_valid_redirection(CustomVector tokens) {
    int n = custom_get_size(&tokens);
    for (int i = 0; i < n; i++) {
        if (strcmp(">", custom_get(&tokens, i)) == 0) {
            if (i == 0 || i == n-1 || n-1-i > 1) return 0;
            if (i != n-1 && strcmp(">", custom_get(&tokens, i+1)) == 0) return 0;
        }
    }
    return 1;
}

int custom_is_valid_ampersand(CustomVector tokens) {
    int n = custom_get_size(&tokens);
    for (int i = 0; i < n; i++) {
        if (strcmp("&", custom_get(&tokens, i)) == 0) {
            if (i == 0) return 0;
            if (i != n-1 && strcmp("&", custom_get(&tokens, i+1)) == 0) return 0;
        }
    }
    return 1;
}

char* custom_concat(char* a, char* b) {
    char* ans = (char*) malloc((strlen(a) + strlen(b) + 1) * sizeof(char));
    strcpy(ans, a);
    strcat(ans, b);
    return ans;
}

void custom_show_error() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

int custom_execute_command(CustomVector tokens) {
    char* command = custom_get(&tokens, 0);

    if (strcmp(command, "exit") == 0) {
        if (custom_get_size(&tokens) != 1) custom_show_error();
        else exit(0);
        return -1;
    }

    if (
        strcmp(command, "cd") == 0
    ) {
        if (custom_get_size(&tokens) != 2) custom_show_error();
        else chdir(custom_get(&tokens, 1));
        return -1;
    }

    if (custom_get_size(&tokens) >= 1 && strcmp("path", command) == 0) {
        CustomVector params = create_custom_vector();
        for (int i = 1; i < custom_get_size(&tokens); i++) custom_push_back(&params, custom_get(&tokens, i));
        custom_PATH = params;
        return -1;
    }
    
    int pos = custom_search_key(&tokens, ">");
    if (pos == -1) pos = custom_get_size(&tokens);
    for (int i = 0; i < custom_PATH.size; i++) {
        char* p = custom_concat(custom_get(&custom_PATH, i), custom_concat("/", command));
        if (access(p, X_OK) == 0) {
            char* argv[pos+1];
            for (int i = 0; i < pos; i++) argv[i] = custom_get(&tokens, i);
            argv[pos] = NULL;
            
            int rc = fork();
            if (rc == 0) {
                if (pos != custom_get_size(&tokens)) {
                    close(STDOUT_FILENO);
                    open(
                        custom_get(&tokens, custom_get_size(&tokens) - 1),
                        O_CREAT | O_WRONLY | O_TRUNC,
                        S_IRWXU
                    );
                }
                execv(p, argv);
            }
            return rc;
        }
    }

    custom_show_error();
    return -1;
}

int main(int argc, char* argv[]) {

    FILE* input_file = NULL;
    for (int i = 1; i < argc; i++) {
        FILE* cur_file = fopen(argv[i], "r");

        if (cur_file == NULL) {
            custom_show_error();
            exit(1);
        }

        if (input_file == NULL) {
            input_file = cur_file;
        } else {
            if (!custom_is_same_file(fileno(input_file), fileno(cur_file))) {
                custom_show_error();
                exit(1);
            }
        }
    }

    if (input_file != NULL) stdin = fdopen(fileno(input_file), "r");

    custom_push_back(&custom_PATH, "/bin");

    while (1) {
        if (input_file == NULL) printf("wish> ");
        char* line = NULL;
        size_t n = 0;

        int len = getline(&line, &n, stdin);
        if (len == -1) {
            if (input_file == NULL) continue;
            break;
        }
        line[len-1] = '\0';

        CustomVector tokens = custom_parse(line);

        if (!custom_is_valid_ampersand(tokens)) {
            continue;
        }

        int commandsCount = 1;
        for (int i = 0; i < custom_get_size(&tokens); i++) {
            if (strcmp("&", custom_get(&tokens, i)) == 0) commandsCount++;
        }

        CustomVector command = create_custom_vector();
        int processIds[commandsCount];
        int sz = 0;
        for (int i = 0; i < custom_get_size(&tokens); i++) {
            if (strcmp("&", custom_get(&tokens, i)) != 0) {
                custom_push_back(&command, custom_get(&tokens, i));
            } else {
                continue;
            }
            if (
                i == custom_get_size(&tokens) - 1 ||
                strcmp("&", custom_get(&tokens, i+1)) == 0
            ) {
                if (!custom_is_valid_redirection(command)) {
                    custom_show_error();
                    break;
                }
                int res = custom_execute_command(command);
                if (res != -1) processIds[sz++] = res;
                command = create_custom_vector();
            }
        }

        for (int i = 0; i < sz; i++) waitpid(processIds[i], NULL, 0);

        free(line);
        custom_destroy(&tokens);
        custom_destroy(&command);
    }
    return 0;
}

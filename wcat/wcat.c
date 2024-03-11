#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[1024];

    if (argc == 1) {
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r");
        
        if (fp == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(fp);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int count;
    char character;

    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            printf("wunzip: cannot open file'\n");
            return 1;
        }

        while (fread(&count, sizeof(int), 1, file) && fread(&character, sizeof(char), 1, file)) {
            for (int j = 0; j < count; j++) {
                printf("%c", character);
            }
        }

        fclose(file);
    }

    return 0;
}

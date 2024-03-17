#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "wzip: file1 [file2 ...]\n");
        return 1;
    }

    int count = 0;
    char prev_char = '\0';

    for (int file_index = 1; file_index < argc; file_index++) {
        FILE *input_file = fopen(argv[file_index], "r");
        if (!input_file) {
            fprintf(stderr, "wzip: cannot open file \n");
            return 1;
        }

        int current_char;

        while ((current_char = fgetc(input_file)) != EOF) {
            if (current_char == prev_char) {
                count++;
            } else {
                if (count > 0) {
                    fwrite(&count, sizeof(int), 1, stdout);
                    fwrite(&prev_char, sizeof(char), 1, stdout);
                }
                prev_char = current_char;
                count = 1;
            }
        }

        fclose(input_file);
    }

    if (count > 0) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&prev_char, sizeof(char), 1, stdout);
    }

    return 0;
}

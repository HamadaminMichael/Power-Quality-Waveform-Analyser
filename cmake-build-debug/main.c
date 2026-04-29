#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.csv>\n", argv[0]);
        return 1;
    }

    printf("Input file: %s\n", argv[1]);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.csv>\n", argv[0]);
        return 1;
    }

    size_t n = 0;
    WaveformSample *samples = load_csv(argv[1], &n);
    if (samples == NULL) {
        return 1;
    }

    printf("Loaded %zu samples from %s\n", n, argv[1]);

    free(samples);
    return 0;
}
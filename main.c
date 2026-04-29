#include <stdio.h>
#include "waveform.h"
#include "io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.csv>\n", argv[0]);
        return 1;
    }

    WaveformSample samples[MAX_SAMPLES];
    size_t n = load_csv(argv[1], samples, MAX_SAMPLES);
    if (n == 0) {
        return 1;
    }

    printf("Loaded %zu samples from %s\n", n, argv[1]);
    return 0;
}
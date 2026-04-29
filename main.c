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

    for (int p = 0; p < 3; p++) {
        char name = (char)('A' + p);
        printf("Phase %c -- RMS: %.3f V, Peak-to-peak: %.3f V\n",
               name,
               compute_rms(samples, n, p),
               compute_peak_to_peak(samples, n, p));
    }

    free(samples);
    return 0;
}
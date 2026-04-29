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
    printf("Phase A RMS: %.3f V (nominal %.1f V)\n",
           compute_rms(samples, n, 0), NOMINAL_RMS);
    printf("Phase B RMS: %.3f V (nominal %.1f V)\n",
           compute_rms(samples, n, 1), NOMINAL_RMS);
    printf("Phase C RMS: %.3f V (nominal %.1f V)\n",
           compute_rms(samples, n, 2), NOMINAL_RMS);

    free(samples);
    return 0;
}
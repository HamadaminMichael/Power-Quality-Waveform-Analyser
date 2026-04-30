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

    const char *names[3]    = { "Phase A", "Phase B", "Phase C" };
    PhaseSelector phases[3] = { PHASE_A,   PHASE_B,   PHASE_C   };

    for (int i = 0; i < 3; i++) {
        PhaseMetrics m = compute_phase_metrics(samples, n, phases[i]);
        printf("%s -- RMS: %.3f V, Pk-Pk: %.3f V, DC offset: %.3f V, clipping: %d\n",
               names[i], m.rms, m.peak_to_peak, m.dc_offset, m.clipping_count);
    }

    free(samples);
    return 0;
}
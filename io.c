#include "io.h"
#include "waveform.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 512

static size_t count_data_rows(FILE *fp) {
    char line[MAX_LINE];
    size_t count = 0;

    if (fgets(line, sizeof(line), fp) == NULL) {
        return 0;
    }
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line[0] != '\n' && line[0] != '\r' && line[0] != '\0') {
            count++;
        }
    }
    return count;
}

WaveformSample *load_csv(const char *filename, size_t *out_count) {
    *out_count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open file '%s'\n", filename);
        return NULL;
    }

    size_t n = count_data_rows(fp);
    if (n == 0) {
        fclose(fp);
        return NULL;
    }

    WaveformSample *samples = malloc(n * sizeof(WaveformSample));
    if (samples == NULL) {
        fclose(fp);
        return NULL;
    }

    rewind(fp);

    char line[MAX_LINE];
    fgets(line, sizeof(line), fp);

    size_t i = 0;
    while (i < n && fgets(line, sizeof(line), fp) != NULL) {
        WaveformSample *s = samples + i;
        int parsed = sscanf(line,
                            "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                            &s->timestamp,
                            &s->phase_A_voltage,
                            &s->phase_B_voltage,
                            &s->phase_C_voltage,
                            &s->line_current,
                            &s->frequency,
                            &s->power_factor,
                            &s->thd_percent);
        if (parsed == 8) {
            i++;
        }
    }

    fclose(fp);
    *out_count = i;
    return samples;
}

int write_results(const char *filename,
                  const WaveformSample *samples,
                  size_t count) {
    if (samples == NULL || count == 0) {
        return 1;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return 1;
    }

    fprintf(fp, "Power Quality Analysis Report\n");
    fprintf(fp, "=============================\n\n");
    fprintf(fp, "Samples analysed: %zu\n", count);
    fprintf(fp, "Window duration:  %.4f s\n",
            samples[count - 1].timestamp - samples[0].timestamp);
    fprintf(fp, "Sample period:    %.4f ms\n\n",
            (samples[count - 1].timestamp - samples[0].timestamp)
                / (double)(count - 1) * 1000.0);

    const char *names[3]    = { "Phase A", "Phase B", "Phase C" };
    PhaseSelector phases[3] = { PHASE_A,   PHASE_B,   PHASE_C   };

    for (int i = 0; i < 3; i++) {
        PhaseMetrics m = compute_phase_metrics(samples, count, phases[i]);
        fprintf(fp, "%s\n", names[i]);
        fprintf(fp, "  RMS voltage:      %8.3f V (nominal %.1f V)\n",
                m.rms, NOMINAL_RMS);
        fprintf(fp, "  Peak-to-peak:     %8.3f V\n", m.peak_to_peak);
        fprintf(fp, "  DC offset:        %8.3f V\n", m.dc_offset);
        fprintf(fp, "  Clipping samples: %d\n", m.clipping_count);
        fprintf(fp, "  Within +/-10%%:    %s\n",
                m.within_tolerance ? "YES" : "NO");
        fprintf(fp, "\n");
    }

    fclose(fp);
    return 0;
}
#include "io.h"
#include "waveform.h"

#include <stdio.h>

#define MAX_LINE 512

size_t load_csv(const char *filename,
                WaveformSample *out_samples,
                size_t max_samples) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open file '%s'\n", filename);
        return 0;
    }

    char line[MAX_LINE];
    /* Skip header row */
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }

    size_t i = 0;
    while (i < max_samples && fgets(line, sizeof(line), fp) != NULL) {
        WaveformSample *s = &out_samples[i];
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
    return i;
}
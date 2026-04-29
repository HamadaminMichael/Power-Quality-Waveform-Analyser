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
    /* Skip header */
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
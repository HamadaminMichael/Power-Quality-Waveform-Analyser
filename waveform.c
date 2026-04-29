#include "waveform.h"
#include <math.h>
#include <stddef.h>

double compute_rms(const WaveformSample *samples, size_t n, int phase) {
    if (samples == NULL || n == 0) {
        return 0.0;
    }

    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double v;
        if (phase == 0) {
            v = samples[i].phase_A_voltage;
        } else if (phase == 1) {
            v = samples[i].phase_B_voltage;
        } else {
            v = samples[i].phase_C_voltage;
        }
        sum_sq += v * v;
    }
    return sqrt(sum_sq / (double)n);
}

double compute_peak_to_peak(const WaveformSample *samples, size_t n, int phase) {
    if (samples == NULL || n == 0) {
        return 0.0;
    }

    double first;
    if (phase == 0) {
        first = samples[0].phase_A_voltage;
    } else if (phase == 1) {
        first = samples[0].phase_B_voltage;
    } else {
        first = samples[0].phase_C_voltage;
    }

    double vmin = first;
    double vmax = first;
    for (size_t i = 1; i < n; i++) {
        double v;
        if (phase == 0) {
            v = samples[i].phase_A_voltage;
        } else if (phase == 1) {
            v = samples[i].phase_B_voltage;
        } else {
            v = samples[i].phase_C_voltage;
        }
        if (v < vmin) vmin = v;
        if (v > vmax) vmax = v;
    }
    return vmax - vmin;
}
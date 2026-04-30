#ifndef WAVEFORM_H
#define WAVEFORM_H
#include <stddef.h>

#define NOMINAL_RMS 230.0

typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample;

/* phase: 0 = A, 1 = B, 2 = C */
double compute_rms(const WaveformSample *samples, size_t n, int phase);
double compute_peak_to_peak(const WaveformSample *samples, size_t n, int phase);
double compute_dc_offset(const WaveformSample *samples, size_t n, int phase);

#endif
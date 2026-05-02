#ifndef WAVEFORM_H
#define WAVEFORM_H
#include <stddef.h>

/* Constants */
#define NOMINAL_RMS         230.0
#define TOLERANCE           0.10
#define CLIPPING_THRESHOLD  324.9

/* Single instant mesurement */
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

typedef enum { PHASE_A, PHASE_B, PHASE_C } PhaseSelector;

/* Stats computed over many samples for a phase */
typedef struct {
    double rms;
    double peak_to_peak;
    double dc_offset;
    double variance;
    double std_dev;
    int    clipping_count;
    int    within_tolerance;
} PhaseMetrics;

double phase_voltage(const WaveformSample *s, PhaseSelector p);

PhaseMetrics compute_phase_metrics(const WaveformSample *samples,
                                   size_t n,
                                   PhaseSelector p);

#endif
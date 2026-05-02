#include "waveform.h"
#include <math.h>
#include <stddef.h>

/* Pull the right voltage field out of a sample for the requested phase.*/

double phase_voltage(const WaveformSample *s, PhaseSelector p) {
    switch (p) {
        case PHASE_A: return s->phase_A_voltage;
        case PHASE_B: return s->phase_B_voltage;
        case PHASE_C: return s->phase_C_voltage;
    }
    return 0.0;
}

/*Why Two-pass?
*Two passes are used because one-pass formula suffers cancellation when the mean is big compared to the spread*/

PhaseMetrics compute_phase_metrics(const WaveformSample *samples,
                                   size_t n,
                                   PhaseSelector p) {
    PhaseMetrics m = {0};
    if (samples == NULL || n == 0) {
        return m;
    }

    /* Pass 1: sum, sum of squares, min/max, clipping count */
    double sum    = 0.0;
    double sum_sq = 0.0;
    double vmin   = phase_voltage(samples, p);
    double vmax   = vmin;
    int    clip   = 0;

    const WaveformSample *end = samples + n;
    for (const WaveformSample *s = samples; s < end; s++) {
        double v = phase_voltage(s, p);
        sum    += v;
        sum_sq += v * v;
        if (v < vmin) vmin = v;
        if (v > vmax) vmax = v;
        if (fabs(v) >= CLIPPING_THRESHOLD) clip++;
    }

    m.dc_offset       = sum / (double)n;
    m.rms             = sqrt(sum_sq / (double)n);
    m.peak_to_peak    = vmax - vmin;
    m.clipping_count  = clip;
    m.within_tolerance = (m.rms >= NOMINAL_RMS * (1.0 - TOLERANCE)) &&
                         (m.rms <= NOMINAL_RMS * (1.0 + TOLERANCE));

    /* Pass 2: sum of squared deviations from the mean */
    double sum_sq_dev = 0.0;
    for (const WaveformSample *s = samples; s < end; s++) {
        double v   = phase_voltage(s, p);
        double dev = v - m.dc_offset;
        sum_sq_dev += dev * dev;
    }

    m.variance = sum_sq_dev / (double)n;
    m.std_dev  = sqrt(m.variance);

    return m;
}
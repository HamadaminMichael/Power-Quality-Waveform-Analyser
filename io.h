#ifndef IO_H
#define IO_H
#include <stddef.h>
#include "waveform.h"

#define MAX_SAMPLES 2000

size_t load_csv(const char *filename,
                WaveformSample *out_samples,
                size_t max_samples);

#endif
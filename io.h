#ifndef IO_H
#define IO_H
#include <stddef.h>
#include "waveform.h"

WaveformSample *load_csv(const char *filename, size_t *out_count);

#endif
#ifndef IO_H
#define IO_H

#include "waveform.h"

int count_rows(const char *filename);
int csv_memory(const char *filename, CSV_Inputs *memory, int N_MAX);

void write_output(const char *filename, const Results *reslults, int N_phases, const CSV_Inputs *very_expensive_memory, int n);

#endif
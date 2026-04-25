#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "io.h"
#include "waveform.h"

enum{
    ERROR_NONE = 0,
    ERROR_ARGV = 1,
    ERROR_EMPTY_FILE = 2,
    ERROR_MEMORY = 3,
    ERROR_CSV_READ = 4
};

static const struct {
    const char *phase;
    size_t offset;
} 

phase_info[3] = {
    {"A", offsetof(CSV_Inputs, phase_A_voltage)},
    {"B", offsetof(CSV_Inputs, phase_B_voltage)},
    {"C", offsetof(CSV_Inputs, phase_C_voltage)}
};

int main(int argc, char *argv[]) {
    int status = ERROR_NONE;
    CSV_Inputs *very_expensive_memory = NULL;
    if (argc != 2) {
        fprintf(stderr, "Error: Could not open CSV file (atleast you won't get any intruders).\n");
        status = ERROR_ARGV;
        goto cleanup;

    }

    int n = count_rows(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Error: File could not be read or is empty (is anyone in there????).\n");
        status = ERROR_EMPTY_FILE;
        goto cleanup;

    }

    very_expensive_memory = malloc(n * sizeof(CSV_Inputs));

    //Checking memory allocation
    if (very_expensive_memory == NULL) {
        fprintf(stderr, "Error: Could not allocate memory (its too expensive in this economy!)\n");
        status = ERROR_MEMORY;
        goto cleanup;

    }

    int csv = csv_memory(argv[1], very_expensive_memory, n);
    if (csv != n) {
        fprintf(stderr, "Error: Failed to read all rows from the CSV file (might have to bring out the reading glasses).\n");
        status = ERROR_CSV_READ;
        goto cleanup;
    }

    Results outputs[3];
    for (int i = 0; i < 3; i++) {
        outputs[i] = phase_analysis(very_expensive_memory, n, phase_info[i].offset, phase_info[i].phase[0]);
    
    }

    printf("Phase\tDC Offset\tPeak-to-Peak\tRMS\tClipping\tCompliance\n");
    for (int i = 0; i < 3; i++) {
        printf("%-8c %-12.2f %-14.2f %-8.2f %-10d %s\n",
                outputs[i].Phase,
                outputs[i].DC_Offset,
                outputs[i].Peak_to_Peak,
                outputs[i].RMS,
                outputs[i].Clipping,
                outputs[i].Compliance ? "Yes" : "No");
    
    }
    write_output("output.txt", outputs, 3, very_expensive_memory, n);
    fprintf(stdout, ("\nProcess Complete: view output.txt! YAYYY!!!!!!!!"));
    
    cleanup:
    free(very_expensive_memory);
    very_expensive_memory = NULL;

    return 0;

}
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "io.h"
#include "waveform.h"


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
    CSV_Inputs *very_expensive_memory = NULL;
    if (argc != 2) {
        fprintf(stderr, "Error: Could not open CSV file (atleast you won't get any intruders).\n");
        return 1;

    }

    int n = count_rows(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Error: File could not be read or is empty (is anyone in there????).\n");
        return 2;

    }

    very_expensive_memory = malloc(n * sizeof(CSV_Inputs));

    //Checking memory allocation
    if (very_expensive_memory == NULL) {
        fprintf(stderr, "Error: Could not allocate memory (its too expensive in this economy!)\n");
        return 3;

    }

    int csv = csv_memory(argv[1], very_expensive_memory, n);
    if (csv != n) {
        fprintf(stderr, "Error: Failed to read all rows from the CSV file (might have to bring out the reading glasses).\n");
        return 4;
    }
    


    
    free(very_expensive_memory);

    return 0;

}
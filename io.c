#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "waveform.h"

#define MAX_LINE_LENGTH 256

int count_rows(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Couldn't open CSV file (get better at hide and seek!!!!!).\n");
        return 1;
    
    }

    char line [MAX_LINE_LENGTH];

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n' && line[0] != '\r' && line[0] != '\0') {
            count++;
        }
        
    }

    fclose(file);
    return count;

}

int csv_memory(const char *filename, CSV_Inputs *memory, int N_MAX) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Couldn't open CSV file (get better at hide and seek!!!!!).\n");
        return 0;
    
    }

    char line[MAX_LINE_LENGTH];

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    int i = 0;
    while (i < N_MAX && fgets(line, sizeof(line), file) != NULL) {
        if (line[0] =='\n' || line[0] == '\r' || line[0] == '\0') {
            continue;
        }

        CSV_Inputs *m = &memory[i];
        int fields = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            &m->Frequency,
            &m->line_current,
            &m->power_factor,
            &m->THD_percent,
            &m->timestamp,
            &m->phase_A_voltage,
            &m->phase_B_voltage,
            &m->phase_C_voltage
        );
        
        if (fields ==8) {
            i++;
        }
        else {
            fprintf(stderr, "Warning: Cannot read row %d read %d/8 fields .\n", i + 1, fields);

        }

    }
    fclose(file);
    return i;

}

static void stats(const CSV_Inputs *memory, int n, int field_offset, double *min_output, double *max_output, double *avg_output) {
    double output = *(const double *)((const char *)&memory[0] + field_offset);
    double N_MIM = output, N_MAX = output, N_SUM = 0.0;

    for (int i = 0; i < n; i++) {
        output = *(const double *)((const char *)&memory[i] + field_offset);
        if (output < N_MIM) {
            N_MIM = output;
        }
        if (output > N_MAX) {
            N_MAX = output;
        }
        N_SUM += output;
    }
    
    *min_output = N_MIM;
    *max_output = N_MAX;
    *avg_output = N_SUM / n;

}

void write_output(const char *filename, const Results *results, int N_phases, const CSV_Inputs *memory, int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Couldn't open output file for writing (did you forget to create it????).\n");
        return;
    
    }

    fprintf(file,"Dataset: %d Amount of data parsed at %.0f samples/s\n", n , SAMPLE_RATE, (double)n / SAMPLE_RATE);
    fprintf(file, "Phase Analysis (EN 50160 band: %.0f V - %.0f V)\n" , V_MIN, V_MAX);

    int total_clipping = 0;
    int all_compliant = 1;

    for (int i = 0; i <N_phases; i++) {
        const Results *r = &results[i];
        fprintf(file, "Phase %c:\n", r->Phase);
        fprintf(file, "  DC Offset: %.2f V\n", r->DC_Offset);
        fprintf(file, "  Peak-to-Peak: %.2f V\n", r->Peak_to_Peak);
        fprintf(file, "  RMS: %.3f V\n", r->RMS, r-> Compliance ? "Yes" : "No");
        fprintf(file, "  Clipping: %s\n", r->Clipping);
    
    }

    double F_MIN, F_MAX, F_AVG;
    double PF_MIN, PF_MAX, PF_AVG;
    double THD_MIN, THD_MAX, THD_AVG;

    stats(memory, n, (int)((const char *)&memory[0].Frequency - (const char *)&memory[0]), &F_MIN, &F_MAX, &F_AVG);
    stats(memory, n, (int)((const char *)&memory[0].power_factor - (const char *)&memory[0]), &PF_MIN, &PF_MAX, &PF_AVG);
    stats(memory, n, (int)((const char *)&memory[0].THD_percent - (const char *)&memory[0]), &THD_MIN, &THD_MAX, &THD_AVG);

    fprintf(file, "Stats Summary:\n");
    fprintf(file, "  Frequency Min: %.2f%%\n", F_MIN);
    fprintf(file, "  Frequency Max: %.2f%%\n", F_MAX);
    fprintf(file, "  Frequency Avg: %.2f%%\n", F_AVG);
    fprintf(file, "  Power Factor Min: %.2f%%\n", PF_MIN);
    fprintf(file, "  Power Factor Max: %.2f%%\n", PF_MAX);
    fprintf(file, "  Power Factor Avg: %.2f%%\n", PF_AVG);
    fprintf(file, "  THD Percent Min: %.2f%%\n", THD_MIN);
    fprintf(file, "  THD Percent Max: %.2f%%\n", THD_MAX);
    fprintf(file, "  THD Percent Avg: %.2f%%\n", THD_AVG, THD_AVG < 5.0 ? "[Good] <5.0%" : THD_AVG < 10.0 ? "[Elevated] <8.0%" : "[Too much] >8.0%");
    fprintf(file, "  Clipping Summary  (threshold |V| >= %.1f V)\n", total_clipping);
    for (int i = 0; i < N_phases; i++) {
        fprintf(file, "Phase %c: %d samples clipped\n", results[i].Phase, results[i].Clipping, results[i].Clipping == 1 ? "[Clipping Detected]" : "[No Clipping]");
    
    }

    fprintf(file, "Total: %d samples clipped\n", total_clipping, total_clipping == 1 ? "[Clipping Detected]" : "[No Clipping]");
    fprintf(file, "Overall Compliance: %s\n", all_compliant ? "Compliant with EN 50160" : "Non-compliant with EN 50160");

    fclose(file);

}
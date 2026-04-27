#include <math.h>
#include <stddef.h>

#include "waveform.h"

static double get_voltage(const CSV_Inputs *memory, int i, int offset) {
    return *(const double *)((const char *)&memory[i] + offset);
}

double calc_dc_offset(const CSV_Inputs *memory, int n, int offset) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += get_voltage(memory, i, offset);
    }
    return sum / n;
}

double calc_peak_to_peak(const CSV_Inputs *memory, int n, int offset) {
    double min = get_voltage(memory, 0, offset);
    double max = min;

    for (int i = 1; i < n; i++) {
        double voltage = get_voltage(memory, i, offset);
        if (voltage < min) {
            min = voltage;
        }
        if (voltage > max) {
            max = voltage;
        }
    }

    return max - min;
}

double calc_rms(const CSV_Inputs *memory, int n, int offset) {
    double sum_squares = 0.0;
    for (int i = 0; i < n; i++) {
        double voltage = get_voltage(memory, i, offset);
        sum_squares += voltage * voltage;
    }
    return sqrt(sum_squares / n);
}

int detect_clipping(const CSV_Inputs *memory, int n, int offset, double threshold) {
    int clipping_count = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(get_voltage(memory, i, offset)) > threshold) {
            clipping_count++;
        }       
    }

    return clipping_count;
}

int check_compliance(double rms) { 
    return (rms >= V_MIN) && (rms <= V_MAX) ? 1 : 0;

}

Results phase_analysis(const CSV_Inputs *memory, int n, int offset, char phase) {
    Results results;
    results.Phase = phase;
    results.DC_Offset = calc_dc_offset(memory, n, offset);
    results.Peak_to_Peak = calc_peak_to_peak(memory, n, offset);
    results.RMS = calc_rms(memory, n, offset);
    results.Clipping = detect_clipping(memory, n, offset, V_NOM * (1.0 + EN_50160));
    results.Compliance = check_compliance(results.RMS);
 
    return results;
}
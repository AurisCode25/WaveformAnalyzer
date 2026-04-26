#ifndef WAVEFORM_H
#define WAVEFORM_H

#define V_MIN 207.0
#define V_MAX 253.0
#define V_NOM 230.0
#define F_NOM 50.0
#define SAMPLE_RATE 5000.0
#define EN_50160 0.10

typedef struct {
 double Frequency;
 double line_current;
 double power_factor;
 double THD_percent;
 double timestamp;
 double phase_A_voltage;
 double phase_B_voltage;
 double phase_C_voltage;


} CSV_Inputs;

typedef struct {
    char Phase;
    double DC_Offset;
    double Peak_to_Peak;
    double RMS;
    int Clipping;
    int Compliance;

} Results;

double calc_dc_offset(const CSV_Inputs *very_expensive_memory, int n, int offset);
double calc_peak_to_peak(const CSV_Inputs *very_expensive_memory, int n, int offset);
double calc_rms(const CSV_Inputs *very_expensive_memory, int n, int offset);
int detect_clipping(const CSV_Inputs *very_expensive_memory, int n, int offset, double threshold);
int check_compliance(const Results *results);

Results phase_analysis(const CSV_Inputs *very_expensive_memory, int n, int offset, char phase);

#endif



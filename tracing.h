#ifndef TRACING
#define TRACING

#include <Arduino.h>

extern bool tracing_output_to_serial;

void tracing_set_output_on_serial(bool output);

#define TRACE_IN() { if (tracing_output_to_serial) trace_in_extended(String(FPSTR(__func__)), String(FPSTR(__FILE__)), __LINE__); }
void trace_in(String function_name);
void trace_in_extended(String function_name, String filename, int line_nbr);

#define TRACE_OUT() { if (tracing_output_to_serial) trace_out_extended(String(FPSTR(__func__)), String(FPSTR(__FILE__)), __LINE__); }
void trace_out(String function_name);
void trace_out_extended(String function_name, String filename, int line_nbr);

#define TRACE_VAR(name, value) { if (tracing_output_to_serial) trace_var_extended(String(FPSTR(__func__)), String(FPSTR(__FILE__)), __LINE__, name, value); }
void trace_var(String function_name, String var_name, String var_value);
void trace_var_extended(String function_name, String filename, int line_nbr, String var_name, String var_value);

#endif TRACING

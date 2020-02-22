#include "tracing.h"

#include <Arduino.h>

#define IN  ">>> "
#define OUT "<<< "
#define VAR "  | "

bool tracing_output_to_serial = true;
String nothing = String();

void trace_extended(const char* in_or_out, String function_name, String filename, int line_nbr, String extra) {
  bool function_name_available = !function_name.equals(emptyString);
  bool filename_available = !filename.equals(emptyString);
  bool line_nbr_available = (line_nbr > 0);

  String trace_string = String(in_or_out);

  if (function_name_available) {
    trace_string += String(function_name);
    trace_string += " ";
  } else {
    trace_string += "<unknown function> ";
  }

  if (filename_available || line_nbr_available) {
    trace_string += "(";
  }

  if (filename_available) {
    trace_string += filename;
  }

  if (line_nbr_available) {
    trace_string += "@";
    trace_string += line_nbr;
  }

  if (filename_available || line_nbr_available) {
    trace_string += ")";
  }

  trace_string += " ";
  trace_string += extra;

  Serial.println(trace_string);
}

String build_variable_tracing(String name, String value) {
  return name + "=" + value;
}

void tracing_set_output_on_serial(bool b) {
  tracing_output_to_serial = b;
}

void trace_in(String function_name) {
  if (!tracing_output_to_serial) return;
  trace_extended(IN, function_name, "", -1, nothing);
}

void trace_in_extended(String function_name, String filename, int line_nbr) {
  if (!tracing_output_to_serial) return;
  trace_extended(IN, function_name, filename, line_nbr, nothing);
}

void trace_out(String function_name) {
  if (!tracing_output_to_serial) return;
  trace_extended(OUT, function_name, "", -1, nothing);
}

void trace_out_extended(String function_name, String filename, int line_nbr) {
  if (!tracing_output_to_serial) return;
  trace_extended(OUT, function_name, filename, line_nbr, nothing);
}

void trace_var(String function_name, String var_name, String var_value) {
  if (!tracing_output_to_serial) return;
  trace_extended(VAR, function_name, "", -1, build_variable_tracing(var_name, var_value));
}

void trace_var_extended(String function_name, String filename, int line_nbr, String var_name, String var_value) {
  if (!tracing_output_to_serial) return;
  trace_extended(VAR, function_name, filename, line_nbr, build_variable_tracing(var_name, var_value));
}

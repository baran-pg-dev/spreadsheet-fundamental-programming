#ifndef FORMULA_ENGINE_H
#define FORMULA_ENGINE_H

#include "sheet.h"
int evaluate_formula(Sheet* sheet, const char* formula, double* result, char* error_msg);
int evaluate_cell(Sheet* sheet, const char* address, double* result, char* error_msg);

#endif

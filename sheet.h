#ifndef SHEET_H
#define SHEET_H

#include "cell.h"

#define MAX_ROWS 100
#define MAX_COLS 26

typedef struct Sheet {
    Cell* cells[MAX_ROWS][MAX_COLS];  //آرایه دو بعدی
    int max_row;              //ذخیره تعداد سطر استفاده شده در شیت         
    int max_col;                  //ذخیره تعداد ستون استفاده شده در شیت     
} Sheet;

Sheet* create_sheet();
void free_sheet(Sheet* sheet);
Cell* get_cell(Sheet* sheet, const char* address);
Cell* get_or_create_cell(Sheet* sheet, const char* address);
void set_cell_value(Sheet* sheet, const char* address, double value);
void set_cell_formula(Sheet* sheet, const char* address, const char* formula);
void print_sheet(Sheet* sheet);
int parse_cell_address(const char* address, int* row, int* col);
void format_cell_address(int row, int col, char* address);

#endif

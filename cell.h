#ifndef CELL_H
#define CELL_H

#define MAX_FORMULA_LEN 256
#define MAX_ADDRESS_LEN 10
#define MAX_ERROR_LEN 256

typedef struct Cell {
    char address[MAX_ADDRESS_LEN];  
    double value;                    
    char formula[MAX_FORMULA_LEN];   //رشته فرمول(در صورت وجود)
    char error_message[MAX_ERROR_LEN]; //پیام خطا
    int has_formula;  //bool               
    int has_error;     //bool              
} Cell;

Cell* create_cell(const char* address);
void free_cell(Cell* cell);

#endif

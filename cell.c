#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cell.h"


Cell* create_cell(const char* address) {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    if (cell == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }
    
    strcpy(cell->address, address);
    cell->value = 0.0;
    cell->formula[0] = '\0';
    cell->error_message[0] = '\0';
    cell->has_formula = 0;
    cell->has_error = 0;
    
    return cell;
}


void free_cell(Cell* cell) {
    if (cell != NULL) {
        free(cell);
    }
}

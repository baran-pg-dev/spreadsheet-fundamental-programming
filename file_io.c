#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "formula_engine.h"

//ذخیره شیت در فایل
int save_sheet(Sheet* sheet, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s' for writing\n", filename);
        return 0;
    }
    
    fprintf(file, "SPREADSHEET_V1\n");
    
    //ذخیره سلول های دارای مقدار
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            Cell* cell = sheet->cells[i][j];
            if (cell != NULL) {
                if (cell->has_formula) {
                    fprintf(file, "%s=%s\n", cell->address, cell->formula);
                } else {
                    fprintf(file, "%s=%.10f\n", cell->address, cell->value);
                }
            }
        }
    }
    
    fclose(file);
    printf("Sheet saved to '%s'\n", filename);
    return 1;
}

//بارگذاری شیت از فایل
int load_sheet(Sheet* sheet, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s' for reading\n", filename);
        return 0;
    }
    
    char line[512];
    
    //خواندن هدر
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error: Invalid file format\n");
        fclose(file);
        return 0;
    }
    
    if (strncmp(line, "SPREADSHEET_V1", 14) != 0) {
        fprintf(stderr, "Error: Invalid file format\n");
        fclose(file);
        return 0;
    }
    
    //خواندن سلول ها
    while (fgets(line, sizeof(line), file) != NULL) {
        //حذف خط جدید
        line[strcspn(line, "\n")] = 0;//پیدا کردن موقعیت اولین وقوع(char span)
        char* equals = strchr(line, '=');
        if (equals == NULL) continue;
        
        *equals = '\0';
        char* address = line;
        char* value_or_formula = equals + 1;
        char* endptr;
        double value = strtod(value_or_formula, &endptr);
        
        if (*endptr == '\0' && endptr != value_or_formula) {
            //عدد
            set_cell_value(sheet, address, value);
        } else {
            //فرمول
            set_cell_formula(sheet, address, value_or_formula);
        }
    }
    
    fclose(file);
    printf("Sheet loaded from '%s'\n", filename);
    return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sheet.h"

Sheet* create_sheet() {
    Sheet* sheet = (Sheet*)malloc(sizeof(Sheet));
    if (sheet == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }
    
    // مقداردهی اولیه سلول ها
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            sheet->cells[i][j] = NULL;
        }
    }
    
    sheet->max_row = 0;
    sheet->max_col = 0;
    
    return sheet;
}

void free_sheet(Sheet* sheet) {
    if (sheet == NULL) return;
    //بررسی تمام سلول های شیت 
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            if (sheet->cells[i][j] != NULL) {
                free_cell(sheet->cells[i][j]); //فراخوانی تابع در صورت نال نبودن سلول
            }
        }
    }
    free(sheet);
}

//تجزیه آدرس سلول به اندیس های سطر و ستون
int parse_cell_address(const char* address, int* row, int* col) {
    if (address == NULL || address[0] == '\0') {
        return 0;
    }
    
    //تجزیه بخش حروفی ستون
    int i = 0;
    *col = 0;
    while (isalpha(address[i])) {
        /*
        تبدیل حرف ستون به اندیس عددی با در نظر گرفتن به عنوان یک سیستم عددی با پایه 26 
        چون در الفبای انگلیسی 26 حرف وجود دارد*/
        *col = *col * 26 + (toupper(address[i]) - 'A');
        i++;
    }
    
    if (i == 0) return 0; 
    
    //تجزیه بخش عددی سطر
    *row = 0;
    while (isdigit(address[i])) {
        *row = *row * 10 + (address[i] - '0');
        i++;
    }
    
    if (*row == 0) return 0; 
    
    (*row)--;  //تبدیل آدرس indexed-1 به indexed-0
    
    return 1;
}

void format_cell_address(int row, int col, char* address) {
    int idx = 0;
    
    //تبدیل ستون به حروف
    if (col >= 26) {
        address[idx++] = 'A' + (col / 26) - 1;
    }
    address[idx++] = 'A' + (col % 26);
    
    //تبدیل سطر به عدد
    sprintf(&address[idx], "%d", row + 1);
}

Cell* get_cell(Sheet* sheet, const char* address) {
    int row, col;
    if (!parse_cell_address(address, &row, &col)) {
        return NULL;
    }
    //بررسی اینکه سطر و ستون در محدوده شیت باشند
    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
        return NULL;
    }
    
    return sheet->cells[row][col];
}

Cell* get_or_create_cell(Sheet* sheet, const char* address) {
    int row, col;
    if (!parse_cell_address(address, &row, &col)) {
        return NULL;
    }
    
    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
        return NULL;
    }
    
    if (sheet->cells[row][col] == NULL) {
        sheet->cells[row][col] = create_cell(address);
        if (row > sheet->max_row) sheet->max_row = row; //به روزرسانی تعداد سطر
        if (col > sheet->max_col) sheet->max_col = col;//به روزرسانی تعداد ستون
    }
    
    return sheet->cells[row][col];
}

void set_cell_value(Sheet* sheet, const char* address, double value) {
    Cell* cell = get_or_create_cell(sheet, address);//ایجاد سلول در صورت وجود نداشتن
    if (cell != NULL) {
        cell->value = value;
        cell->has_formula = 0;
        cell->has_error = 0;
        cell->formula[0] = '\0';
        cell->error_message[0] = '\0';
    }
}
void set_cell_formula(Sheet* sheet, const char* address, const char* formula) {
    Cell* cell = get_or_create_cell(sheet, address);
    if (cell != NULL) {
        strcpy(cell->formula, formula);
        cell->has_formula = 1;//تغییر وضعیت فرمول به حضور
        cell->has_error = 0;
        cell->error_message[0] = '\0';
    }
}

void print_sheet(Sheet* sheet) {
    if (sheet == NULL) return;
    
    printf("\n");
//چاپ ستون
    printf("%-8s", "");
    for (int j = 0; j <= sheet->max_col && j < MAX_COLS; j++) {
        printf("%-12c", 'A' + j);
    }
    printf("\n");
    
//چاپ سطر
    for (int i = 0; i <= sheet->max_row && i < MAX_ROWS; i++) {
        printf("%-8d", i + 1);
        for (int j = 0; j <= sheet->max_col && j < MAX_COLS; j++) {
            Cell* cell = sheet->cells[i][j];
            if (cell != NULL) {
                if (cell->has_error) {
                    printf("%-12s", cell->error_message[0] != '\0' ? cell->error_message : "ERROR");
                } else if (cell->has_formula) {
                    printf("%-12.4f", cell->value);
                } else {
                    printf("%-12.4f", cell->value);
                }
            } else {
                printf("%-12s", "");//سلول خالی
            }
        }
        printf("\n");
    }
    printf("\n");
}

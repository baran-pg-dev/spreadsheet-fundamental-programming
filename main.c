#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sheet.h"
#include "formula_engine.h"
#include "file_io.h"
#include "text_analyzer.h"

void print_help();

int main() {
    Sheet* sheet = create_sheet();
    if (sheet == NULL) {
        fprintf(stderr, "Failed to create spreadsheet\n");
        return 1;
    }
    
    printf("Welcome to Spreadsheet Calculator!\n");
    printf("Type 'HELP' for available commands.\n");
    
    char command[512];
    
    while (1) {
        printf("\n> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        //حذف خط جدید enter
        command[strcspn(command, "\n")] = 0;
        char cmd[50], arg1[100], arg2[300];//تقسیم دستور
        int num_args = sscanf(command, "%s %s %[^\n]", cmd, arg1, arg2);
        
        if (num_args < 1) continue;//اگر کاربر فقط اینتر زده باشه
        
        //تبدیل دستور به حروف بزرگ
        for (int i = 0; cmd[i]; i++) {
            cmd[i] = toupper(cmd[i]);
        }
        
        if (strcmp(cmd, "EXIT") == 0 || strcmp(cmd, "QUIT") == 0) {
            break;
        } else if (strcmp(cmd, "HELP") == 0) {
            print_help();
        } else if (strcmp(cmd, "SHOW") == 0) {
            print_sheet(sheet);
        } else if (strcmp(cmd, "SET") == 0 && num_args >= 3) {
            //تبدیل آدرس سلول به حروف بزرک
            for (int i = 0; arg1[i]; i++) {
                arg1[i] = toupper(arg1[i]);
            }
            
            if (arg2[0] == '=') {
                //فرمول
                set_cell_formula(sheet, arg1, arg2 + 1);
                printf("Formula set for cell %s\n", arg1);
            } else {
                //مقدار
                double value = atof(arg2);
                set_cell_value(sheet, arg1, value);
                printf("Value set for cell %s: %.4f\n", arg1, value);
            }
        } else if (strcmp(cmd, "GET") == 0 && num_args >= 2) {
            //تبدیل آدرس سلول به حروف بزرگ
            for (int i = 0; arg1[i]; i++) {
                arg1[i] = toupper(arg1[i]);
            }
            
            Cell* cell = get_cell(sheet, arg1);
            if (cell == NULL) {
                printf("Cell %s is empty (value: 0)\n", arg1);
            } else if (cell->has_error) {
                printf("Cell %s has error\n", arg1);
            } else if (cell->has_formula) {
                printf("Cell %s: %.4f (formula: %s)\n", arg1, cell->value, cell->formula);
            } else {
                printf("Cell %s: %.4f\n", arg1, cell->value);
            }
        } else if (strcmp(cmd, "EVAL") == 0 && num_args >= 2) {
            //تبدیل آدرس سلول به حروف بزرگ
            for (int i = 0; arg1[i]; i++) {
                arg1[i] = toupper(arg1[i]);
            }
            
            Cell* cell = get_cell(sheet, arg1);
            if (cell == NULL) {
                printf("Cell %s is empty\n", arg1);
            } else if (!cell->has_formula) {
                printf("Cell %s has no formula\n", arg1);
            } else {
                double result;
                char error_msg[256];
                if (evaluate_formula(sheet, cell->formula, &result, error_msg)) {
                    cell->value = result;
                    cell->has_error = 0;
                    cell->error_message[0] = '\0';
                    printf("Cell %s evaluated: %.4f\n", arg1, result);
                } else {
                    cell->has_error = 1;
                    strcpy(cell->error_message, error_msg);
                    printf("Error evaluating cell %s: %s\n", arg1, error_msg);
                }
            }
        } else if (strcmp(cmd, "EVALALL") == 0) {
            int count = 0;//تعداد سلول هایی که با موفقیت محاسبه شدند
            int errors = 0;
            
            for (int i = 0; i < MAX_ROWS; i++) {
                for (int j = 0; j < MAX_COLS; j++) {
                    Cell* cell = sheet->cells[i][j];
                    if (cell != NULL && cell->has_formula) {
                        double result;
                        char error_msg[256];
                        if (evaluate_formula(sheet, cell->formula, &result, error_msg)) {
                            cell->value = result;
                            cell->has_error = 0;
                            cell->error_message[0] = '\0';
                            count++;
                        } else {
                            cell->has_error = 1;
                            strcpy(cell->error_message, error_msg);
                            errors++;
                            printf("Error in cell %s: %s\n", cell->address, error_msg);
                        }
                    }
                }
            }
            
            printf("Evaluated %d cells (%d errors)\n", count, errors);
        } else if (strcmp(cmd, "SAVE") == 0 && num_args >= 2) {
            save_sheet(sheet, arg1);
        } else if (strcmp(cmd, "LOAD") == 0 && num_args >= 2) {
            load_sheet(sheet, arg1);
        } else if (strcmp(cmd, "ANALYZE") == 0 && num_args >= 2) {
            FileStats stats;
            if (analyze_text_file(arg1, &stats)) {
                print_stats(&stats);
                if (write_report(&stats, "report.txt")) {
                    printf("Report saved to 'report.txt'\n");
                }
            }
        } else {
            printf("Unknown command. Type 'HELP' for available commands.\n");
        }
    }
    
    printf("\nGoodbye!\n");
    free_sheet(sheet);
    return 0;
}
void print_help() {
    printf("\n=== Spreadsheet Calculator ===\n");
    printf("Commands:\n");
    printf("  SET <cell> <value>     - Set cell to a numeric value (e.g., SET A1 5)\n");
    printf("  SET <cell> =<formula>  - Set cell to a formula (e.g., SET A2 =sin(A1))\n");
    printf("  GET <cell>             - Get cell value\n");
    printf("  EVAL <cell>            - Evaluate and update cell with formula\n");
    printf("  EVALALL                - Evaluate all cells with formulas\n");
    printf("  SHOW                   - Display the spreadsheet\n");
    printf("  SAVE <filename>        - Save spreadsheet to file\n");
    printf("  LOAD <filename>        - Load spreadsheet from file\n");
    printf("  ANALYZE <filename>     - Analyze text file and create report\n");
    printf("  HELP                   - Show this help message\n");
    printf("  EXIT                   - Exit the program\n");
    printf("\nSupported functions:\n");
    printf("  sin, cos, tan, cot, sinh, cosh, tanh\n");
    printf("  sqrt, abs, exp, ln, log, pow(x,y)\n");
    printf("\nExamples:\n");
    printf("  SET A1 5\n");
    printf("  SET A2 =sin(A1)\n");
    printf("  SET A3 =tan(A5+A1)*ln(sinh(A2))\n");
    printf("  SET B1 =pow(A1,2)+sqrt(16)\n");
    printf("  EVALALL\n");
    printf("  SHOW\n\n");
}
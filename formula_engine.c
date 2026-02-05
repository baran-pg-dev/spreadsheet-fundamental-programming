#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //isspace
#include <math.h>
#include "formula_engine.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//متغیر های سراسری
static const char* formula_position;//اشاره گر بهمکان فعلی در فرمول
static Sheet* current_sheet;//اشاره گر به شیت فعلی
static char* current_error;//اشاره گر به رشته ای که پیام های خطا در آن ذخیره میشه

double parse_expression();
double parse_term();
double parse_factor();
double parse_function();

//نادیده گرفتن فاصله ها و تب ها و خط جدید
void skip_whitespace() {
    while (isspace(*formula_position)) {
        formula_position++;
    }
}

// مثل sin(x), sqrt(x), pow(x,y)
double parse_function() {
    char func_name[20];
    int idx = 0;//شمارش طول نام تابع
    
    // خواندن نام تابع
    while (isalpha(*formula_position) && idx < 19) {
        func_name[idx++] = tolower(*formula_position);
        formula_position++;
    }
    func_name[idx] = '\0';
    
    skip_whitespace();
    
    //پرانتز
    if (*formula_position != '(') {
        sprintf(current_error, "Expected '(' after function name");
        return 0.0;//خطا
    }
    formula_position++;  //نادیده گرفتن پرانتز
    
    //داخل پرانتز
    double arg1 = parse_expression();
    if (current_error[0] != '\0') return 0.0;
    
    skip_whitespace();
    
    //چک کردن آرگومان دوم (توان)
    double arg2 = 0.0;
    int has_two_args = 0;
    if (*formula_position == ',') {
        formula_position++;
        has_two_args = 1;
        arg2 = parse_expression();
        if (current_error[0] != '\0') return 0.0;
        skip_whitespace();
    }
    
    if (*formula_position != ')') {
        sprintf(current_error, "Expected ')' after function arguments");
        return 0.0;
    }
    formula_position++;  //نادیده گرفتن پرانتز
    
    //محاسبه نتیجه
    if (strcmp(func_name, "sin") == 0) {
        return sin(arg1);
    } else if (strcmp(func_name, "cos") == 0) {
        return cos(arg1);
    } else if (strcmp(func_name, "tan") == 0) {
        return tan(arg1);
    } else if (strcmp(func_name, "cot") == 0) {
        double tan_val = tan(arg1);
        if (tan_val == 0.0) {
            sprintf(current_error, "Division by zero in cot()");
            return 0.0;
        }
        return 1.0 / tan_val;
    } else if (strcmp(func_name, "sinh") == 0) {
        return sinh(arg1);
    } else if (strcmp(func_name, "cosh") == 0) {
        return cosh(arg1);
    } else if (strcmp(func_name, "tanh") == 0) {
        return tanh(arg1);
    } else if (strcmp(func_name, "sqrt") == 0) {
        if (arg1 < 0) {
            sprintf(current_error, "Square root of negative number");
            return 0.0;
        }
        return sqrt(arg1);
    } else if (strcmp(func_name, "abs") == 0) {
        return fabs(arg1);
    } else if (strcmp(func_name, "exp") == 0) {
        return exp(arg1);
    } else if (strcmp(func_name, "ln") == 0 || strcmp(func_name, "log") == 0) {
        if (arg1 <= 0) {
            sprintf(current_error, "Logarithm of non-positive number");
            return 0.0;
        }
        return log(arg1);
    } else if (strcmp(func_name, "pow") == 0) {
        if (!has_two_args) {
            sprintf(current_error, "pow() requires two arguments");
            return 0.0;
        }
        return pow(arg1, arg2);
    } else {
        sprintf(current_error, "Unknown function: %s", func_name);
        return 0.0;
    }
}

//تجزیه عناصر
double parse_factor() {
    skip_whitespace();
    
    //در نظر گرفتن علامت منفی
    if (*formula_position == '-') {
        formula_position++;
        return -parse_factor();  //تجزیه بازگشتی بعد از علامت منفی
    }
    
    //در نظر گرفتن علامت مثبت
    if (*formula_position == '+') {
        formula_position++;
        return parse_factor();
    }
    
    if (*formula_position == '(') {
        formula_position++; 
        double result = parse_expression();
        if (current_error[0] != '\0') return 0.0;
        
        skip_whitespace();
        if (*formula_position != ')') {
            sprintf(current_error, "Mismatched parentheses");
            return 0.0;
        }
        formula_position++; 
        return result;
    }
    
    if (isalpha(*formula_position)) {
        //ذخیره موقعیت اولیه
        const char* start = formula_position;
        
        //خواندن حروف
        while (isalpha(*formula_position)) {
            formula_position++;
        }
        //اگر عدد وجود داشته باشه آدرس سلوله
        if (isdigit(*formula_position)) {
            formula_position = start; 
            //خواندن آدرس سلول
            char cell_addr[MAX_ADDRESS_LEN];
            int idx = 0;
            while ((isalpha(*formula_position) || isdigit(*formula_position)) && idx < MAX_ADDRESS_LEN - 1) {
                cell_addr[idx++] = *formula_position;
                formula_position++;
            }
            cell_addr[idx] = '\0';
            
            double value;
            // Preserve parser/evaluator global state during nested cell evaluation
            const char* saved_pos = formula_position;
            Sheet* saved_sheet = current_sheet;
            char* saved_error = current_error;

            if (!evaluate_cell(current_sheet, cell_addr, &value, current_error)) {
                return 0.0;
            }

            // Restore parser/evaluator state after nested evaluation
            formula_position = saved_pos;
            current_sheet = saved_sheet;
            current_error = saved_error;
            return value;
        } else {//اگر بعد از حروف عددی نباشه
            formula_position = start; 
            return parse_function();
        }
    }
    
    // مدیریت اعداد
    if (isdigit(*formula_position) || *formula_position == '.') {
        char* end;
        double value = strtod(formula_position, &end);//تبدیل رشته به عدد
        formula_position = end;
        return value;
    }
    
    sprintf(current_error, "Invalid expression");
    return 0.0;
}
//تجزیه ضرب و تقسیم
double parse_term() {
    double result = parse_factor();
    if (current_error[0] != '\0') return 0.0;
    while (1) {
        skip_whitespace();
        
        if (*formula_position == '*') {
            //عملیات ضرب
            formula_position++;
            double next_factor = parse_factor();
            if (current_error[0] != '\0') return 0.0;
            result = result * next_factor;
            
        } else if (*formula_position == '/') {
            //عملیات تقسیم
            formula_position++;
            double next_factor = parse_factor();
            if (current_error[0] != '\0') return 0.0;
            
            if (next_factor == 0.0) {//خطای تقسیم بر صفر
                sprintf(current_error, "Division by zero");
                return 0.0;
            }
            result = result / next_factor;
            
        } else {

            break;
        }
    }
    
    return result;
}

//تحلیل جمع و تفریق
double parse_expression() {
    double result = parse_term();//پردازش ضرب یا تقسیم
    if (current_error[0] != '\0') return 0.0;
    while (1) {
        skip_whitespace();
        
        if (*formula_position == '+') {
            //عملیات جمع
            formula_position++;
            double next_term = parse_term();
            if (current_error[0] != '\0') return 0.0;
            result = result + next_term;
            
        } else if (*formula_position == '-') {
            //عملیات تفریق
            formula_position++;
            double next_term = parse_term();
            if (current_error[0] != '\0') return 0.0;
            result = result - next_term;
            
        } else {
            break;
        }
    }
    
    return result;
}

//محاسبه
int evaluate_formula(Sheet* sheet, const char* formula, double* result, char* error_msg) {
    formula_position = formula;
    current_sheet = sheet;
    current_error = error_msg;
    error_msg[0] = '\0';  //پاک کردن پیام ارور
    
    //تجزیه و محاسبه فرمول
    *result = parse_expression();
    
    //syntax error
    skip_whitespace();
    if (*formula_position != '\0' && error_msg[0] == '\0') {
        sprintf(error_msg, "Unexpected character in expression");
        return 0;
    }
    
    return (error_msg[0] == '\0');
}
//محاسبه مقدار
int evaluate_cell(Sheet* sheet, const char* address, double* result, char* error_msg) {
    Cell* cell = get_cell(sheet, address);
    
    if (cell == NULL) {
        *result = 0.0;
        return 1;
    }
    
    if (cell->has_error) {
        sprintf(error_msg, "Cell %s has error", address);
        return 0;
    }
    
    if (!cell->has_formula) {
        *result = cell->value;
        return 1;
    }
    
    if (!evaluate_formula(sheet, cell->formula, result, error_msg)) {
        return 0;
    }
    
    return 1;
}

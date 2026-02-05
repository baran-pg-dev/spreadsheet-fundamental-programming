# Spreadsheet Calculator - KNTU

## Project Goal
This project implements an Excel-like spreadsheet calculator in C language. It provides a platform for managing cells, formulas, and mathematical expressions with error handling.

## Capabilities

### Core Features
- **Cell Management**: Store and manage cell values in a spreadsheet grid (up to 100 rows × 26 columns)
- **Mathematical Expressions**: Calculate expressions in cells using standard operators (+, -, *, /, parentheses)
- **Advanced Functions**: Support for mathematical functions:
  - Trigonometric: `sin`, `cos`, `tan`, `cot`
  - Hyperbolic: `sinh`, `cosh`, `tanh`
  - Other: `sqrt`, `abs`, `exp`, `ln`/`log`, `pow(x,y)`
- **Cell References**: Reference other cells in formulas (e.g., `A2 = sin(A1)`)
- **Formula Engine**: Parse and evaluate complex mathematical expressions
- **Error Handling**:
  - Division by zero
  - Unknown functions
  - Invalid cell references
  - Mismatched parentheses
  - Range errors (sqrt of negative, log of non-positive)
- **File I/O**: Save and load spreadsheet data to/from files

### Additional Features
- Interactive command-line interface
- Real-time formula evaluation
- Clear error messages for all error types
- Expandable table structure

## Project Structure

yaldaee
├── main.c              # Main program with command interface
├── cell.h              # Cell structure definition
├── cell.c              # Cell management functions
├── sheet.h             # Spreadsheet structure definition
├── sheet.c             # Spreadsheet operations
├── formula_engine.h    # Formula evaluation interface
├── formula_engine.c    # Expression parser and calculator
├── file_io.h           # File operations interface
├── file_io.c           # Save/load functionality
└── README.md           # This file
```

## How to Build

### Windows (PowerShell)
```powershell
gcc -Wall -Wextra -std=c11 main.c cell.c sheet.c formula_engine.c file_io.c text_analyzer.c -o spreadsheet.exe -lm
```

### Linux/Mac
```bash
gcc -Wall -Wextra -std=c11 main.c cell.c sheet.c formula_engine.c file_io.c text_analyzer.c -o spreadsheet -lm
```

## How to Run

```powershell
./spreadsheet
```

## Usage Examples

### Setting Cell Values
```
> SET A1 5
Value set for cell A1: 5.0000

> SET A2 =sin(A1)
Formula set for cell A2
```

### Evaluating Formulas
```
> EVAL A2
Cell A2 evaluated: -0.9589

> EVALALL
Evaluated 1 cells (0 errors)
```

### Complex Formulas
```
> SET A3 =tan(A5+A1)*ln(sinh(A2))
Formula set for cell A3

> SET B1 =pow(A1,2)+sqrt(16)
Formula set for cell B1

> EVALALL
Evaluated 2 cells (0 errors)
```

### Displaying the Spreadsheet
```
> SHOW

        A           B           C           D           
1       5.0000      29.0000                             
2       -0.9589                                         
3       -1.0472                                         
```

### Saving and Loading
```
> SAVE mysheet.txt
Sheet saved to 'mysheet.txt'

> LOAD mysheet.txt
Sheet loaded from 'mysheet.txt'
```

## Command Reference

| Command | Description | Example |
|---------|-------------|---------|
| `SET <cell> <value>` | Set cell to numeric value | `SET A1 5` |
| `SET <cell> =<formula>` | Set cell to formula | `SET A2 =sin(A1)` |
| `GET <cell>` | Display cell value | `GET A1` |
| `EVAL <cell>` | Evaluate single cell | `EVAL A2` |
| `EVALALL` | Evaluate all formulas | `EVALALL` |
| `SHOW` | Display spreadsheet | `SHOW` |
| `SAVE <file>` | Save to file | `SAVE data.txt` |
| `LOAD <file>` | Load from file | `LOAD data.txt` |
| `HELP` | Show help message | `HELP` |
| `EXIT` | Exit program | `EXIT` |

## Input/Output Sample

### Sample Session
```
Welcome to Spreadsheet Calculator!
Type 'HELP' for available commands.

> SET A1 5
Value set for cell A1: 5.0000

> SET A2 =sin(A1)
Formula set for cell A2

> SET A5 10
Value set for cell A5: 10.0000

> SET A3 =tan(A5+A1)*ln(sinh(A2))
Formula set for cell A3

> EVALALL
Evaluated 2 cells (0 errors)

> SHOW

        A           B           C           D           
1       5.0000                                          
2       -0.9589                                         
3       -1.0472                                         
4                                                       
5       10.0000                                         

> SAVE example.txt
Sheet saved to 'example.txt'

> EXIT
Goodbye!
```

## Error Handling Examples

### Division by Zero
```
> SET A1 =5/0
Formula set for cell A1
> EVAL A1
Error evaluating cell A1: Division by zero
```

### Invalid Function
```
> SET A1 =unknown(5)
Formula set for cell A1
> EVAL A1
Error evaluating cell A1: Unknown function: unknown
```

### Mathematical Errors
```
> SET A1 =sqrt(-5)
Formula set for cell A1
> EVAL A1
Error evaluating cell A1: Square root of negative number
```

## Extra Sample
gcc -o spreadsheet.exe main.c cell.c sheet.c formula_engine.c file_io.c text_analyzer.c -lm ; echo "SET A1 5`nSET A2 =sin(A1)`nSET A5 10`nSET A3 =tan(A5+A1)*ln(sinh(A2))`nEVALALL`nSHOW`nEXIT" | .\spreadsheet.exe

## Author
Baran Poorgoodarzi 40417843
Yekta Shahidi 40420513
Roxana Zakeri 40419293


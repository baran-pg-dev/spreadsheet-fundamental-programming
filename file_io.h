#ifndef FILE_IO_H
#define FILE_IO_H

#include "sheet.h"

int save_sheet(Sheet* sheet, const char* filename);
int load_sheet(Sheet* sheet, const char* filename);

#endif

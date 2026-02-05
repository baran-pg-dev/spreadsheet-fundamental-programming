#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "text_analyzer.h"

int analyze_text_file(const char* filename, FileStats* stats) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s' for reading\n", filename);
        return 0;
    }
    
    strncpy(stats->filename, filename, sizeof(stats->filename) - 1);
    stats->filename[sizeof(stats->filename) - 1] = '\0';
    stats->line_count = 0;
    stats->word_count = 0;
    stats->char_count = 0;
    
    int ch;
    int in_word = 0;  
    int line_started = 0;  
    //شمارش کرکتر
    while ((ch = fgetc(file)) != EOF) {
        stats->char_count++;
        
        //شمارش خطوط
        if (ch == '\n') {
            stats->line_count++;
            in_word = 0;
            line_started = 0;
        } else {
            if (isspace(ch) || ch == '\t') {
                in_word = 0;
            } else {
                if (!in_word) {
                    stats->word_count++;
                    in_word = 1;
                }
                line_started = 1;
            }
        }
    }
    //خط آخر
    if (line_started || stats->char_count > 0) {
        if (stats->char_count > 0 && line_started) {
            stats->line_count++;
        }
    }
    
    fclose(file);
    return 1;
}
int write_report(const FileStats* stats, const char* report_filename) {
    FILE* report = fopen(report_filename, "w");
    if (report == NULL) {
        fprintf(stderr, "Error: Cannot create report file '%s'\n", report_filename);
        return 0;
    }
    
    fprintf(report, "--- File Analysis Report ---\n");
    fprintf(report, "Filename: %s\n", stats->filename);
    fprintf(report, "Lines: %d\n", stats->line_count);
    fprintf(report, "Words: %d\n", stats->word_count);
    fprintf(report, "Characters: %d\n", stats->char_count);
    
    fclose(report);
    return 1;
}

void print_stats(const FileStats* stats) {
    printf("\n--- File Analysis Report ---\n");
    printf("Filename: %s\n", stats->filename);
    printf("Lines: %d\n", stats->line_count);
    printf("Words: %d\n", stats->word_count);
    printf("Characters: %d\n", stats->char_count);
    printf("\n");
}

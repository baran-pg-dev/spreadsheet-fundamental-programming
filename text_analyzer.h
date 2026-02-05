#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

typedef struct {
    char filename[256];
    int line_count;
    int word_count;
    int char_count;
} FileStats;

int analyze_text_file(const char* filename, FileStats* stats);
int write_report(const FileStats* stats, const char* report_filename);
void print_stats(const FileStats* stats);

#endif

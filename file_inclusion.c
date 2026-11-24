#include <stdio.h>
#include <string.h>
#include "preprocessor.h"

static void copy_file(const char *filename, FILE *out) {
    FILE *in = fopen(filename, "r");
    if (!in) {
        char path[256];
        sprintf(path, "/usr/include/%s", filename);
        in = fopen(path, "r");
        if (!in) return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), in)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;

        if (strncmp(p, "#include", 8) == 0) {
            char *start = strpbrk(p + 8, "<\"");
            char *end = start ? strpbrk(start + 1, ">\"") : NULL;
            if (start && end) {
                *end = '\0';
                copy_file(start + 1, out);
                fputc('\n', out);
                continue;
            }
        }
        fputs(line, out);
    }
    fclose(in);
}

void process_includes(const char *in_file, const char *out_file) {
    FILE *out = fopen(out_file, "w");
    if (!out) return;
    copy_file(in_file, out);
    fclose(out);
}

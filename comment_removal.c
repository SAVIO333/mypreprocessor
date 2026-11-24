#include <stdio.h>
#include "preprocessor.h"

void remove_comments(const char *in_file, const char *out_file) {
    FILE *in = fopen(in_file, "r");
    FILE *out = fopen(out_file, "w");
    if (!in || !out) return;

    int c, next, in_str = 0;
    while ((c = fgetc(in)) != EOF) {
        if (c == '"' && !in_str) { in_str = 1; fputc(c, out); }
        else if (c == '"' && in_str) { in_str = 0; fputc(c, out); }
        else if (!in_str && c == '/') {
            next = fgetc(in);
            if (next == '/') {
                while ((c = fgetc(in)) != EOF && c != '\n');
                if (c == '\n') fputc('\n', out);
            } else if (next == '*') {
                while ((c = fgetc(in)) != EOF) {
                    if (c == '\n') fputc('\n', out);
                    if (c == '*' && (next = fgetc(in)) == '/') break;
                }
            } else {
                fputc(c, out);
                if (next != EOF) ungetc(next, in);
            }
        } else {
            fputc(c, out);
        }
    }
    fclose(in);
    fclose(out);
}

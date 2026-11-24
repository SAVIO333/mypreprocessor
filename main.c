#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <inputfile.c>\n", argv[0]);
        return 1;
    }

    char out_file[256];
    strcpy(out_file, argv[1]);
    char *dot = strrchr(out_file, '.');
    if (dot) strcpy(dot, ".i");
    else strcat(out_file, ".i");

    remove_comments(argv[1], "tmp1.c");
    process_includes("tmp1.c", "tmp2.c");
    process_macros("tmp2.c", out_file);

    remove("tmp1.c");
    remove("tmp2.c");

    printf("Generated: %s\n", out_file);
    return 0;
}

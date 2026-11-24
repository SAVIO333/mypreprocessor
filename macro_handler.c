#include <stdio.h>
#include <string.h>
#include "preprocessor.h"

#define MAX 256
#define LIST_MAX 64
#define ARG_MAX 10

typedef struct {
    char name[MAX];
    char params[MAX];
    char body[MAX];
} Macro;

static Macro list[LIST_MAX];
static int count = 0;

static int is_word(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_';
}

static void save_macro(char *line) {
    char *p = line + 7;
    while (*p == ' ' || *p == '\t') p++;

    int i = 0;
    while (is_word(*p)) list[count].name[i++] = *p++;
    list[count].name[i] = '\0';
    list[count].params[0] = '\0';

    if (*p == '(') {
        p++;
        int j = 0;
        while (*p && *p != ')') {
            if (*p != ' ') list[count].params[j++] = *p;
            p++;
        }
        list[count].params[j] = '\0';
        if (*p == ')') p++;
    }

    while (*p == ' ' || *p == '\t') p++;
    strcpy(list[count].body, p);
    list[count].body[strcspn(list[count].body, "\r\n")] = '\0';
    count++;
}

static void expand_simple(const char *src, char *dest) {
    const char *p = src;
    int d = 0;
    while (*p) {
        if (is_word(*p)) {
            char word[MAX];
            int i = 0;
            while (is_word(*p)) word[i++] = *p++;
            word[i] = '\0';

            int found = -1;
            for (int k = 0; k < count; k++)
                if (strcmp(word, list[k].name) == 0 && list[k].params[0] == '\0') found = k;

            const char *rep = (found != -1) ? list[found].body : word;
            strcpy(dest + d, rep);
            d += strlen(rep);
        } else {
            dest[d++] = *p++;
        }
    }
    dest[d] = '\0';
}

static void print_expanded(const char *line, FILE *out) {
    const char *p = line;
    while (*p) {
        if (is_word(*p)) {
            char word[MAX];
            int i = 0;
            while (is_word(*p)) word[i++] = *p++;
            word[i] = '\0';

            int found = -1;
            for (int k = 0; k < count; k++)
                if (strcmp(word, list[k].name) == 0) found = k;

            if (found == -1) {
                fputs(word, out);
            } else if (list[found].params[0] == '\0') {
                fputs(list[found].body, out);
            } else if (*p == '(') {
                p++;
                char raw_args[MAX], exp_args[MAX];
                int j = 0;
                while (*p && *p != ')') raw_args[j++] = *p++;
                raw_args[j] = '\0';
                if (*p == ')') p++;

                expand_simple(raw_args, exp_args);

                char p_copy[MAX], a_copy[MAX];
                strcpy(p_copy, list[found].params);
                strcpy(a_copy, exp_args);

                char *p_names[ARG_MAX], *a_vals[ARG_MAX];
                int n = 0;
                char *pt = strtok(p_copy, ",");
                while (pt && n < ARG_MAX) { p_names[n++] = pt; pt = strtok(NULL, ","); }

                int m = 0;
                char *at = strtok(a_copy, ",");
                while (at && m < ARG_MAX) {
                    while (*at == ' ') at++;
                    a_vals[m++] = at;
                    at = strtok(NULL, ",");
                }

                const char *b = list[found].body;
                while (*b) {
                    if (is_word(*b)) {
                        char tok[MAX];
                        int t = 0;
                        while (is_word(*b)) tok[t++] = *b++;
                        tok[t] = '\0';

                        int idx = -1;
                        for (int k = 0; k < n && k < m; k++)
                            if (strcmp(tok, p_names[k]) == 0) idx = k;

                        fputs((idx != -1) ? a_vals[idx] : tok, out);
                    } else {
                        fputc(*b++, out);
                    }
                }
            }
        } else {
            fputc(*p++, out);
        }
    }
}

void process_macros(const char *in_file, const char *out_file) {
    FILE *in = fopen(in_file, "r");
    FILE *out = fopen(out_file, "w");
    if (!in || !out) return;

    count = 0;
    char line[MAX];
    while (fgets(line, sizeof(line), in)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (strncmp(p, "#define", 7) == 0) save_macro(p);
        else print_expanded(line, out);
    }
    fclose(in);
    fclose(out);
}

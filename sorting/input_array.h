/* Pembaca token int berbatas; satu dataset n (0..128), lalu n nilai.
   Whitespace bebas, token berlebih ditolak, tanpa scanf/VLA. */
#ifndef INPUT_ARRAY_H
#define INPUT_ARRAY_H
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
static int baca_int(int *out) {
    char token[64]; size_t n = 0; int c;
    do { c = getchar(); } while (c != EOF && isspace((unsigned char)c));
    if (c == EOF) return 0;
    do {
        if (n + 1 >= sizeof token) return 0;
        token[n++] = (char)c; c = getchar();
    } while (c != EOF && !isspace((unsigned char)c));
    token[n] = '\0'; char *end; errno = 0;
    long x = strtol(token, &end, 10);
    if (errno || *end || x < INT_MIN || x > INT_MAX) return 0;
    *out = (int)x; return 1;
}
static int baca_array(int a[], int *n) {
    if (!baca_int(n) || *n < 0 || *n > 128) return 0;
    for (int i = 0; i < *n; ++i) if (!baca_int(&a[i])) return 0;
    return 1;
}
static int selesai_input(void) {
    int c; while ((c = getchar()) != EOF) if (!isspace((unsigned char)c)) return 0;
    return 1;
}
static void tampil_array(const int a[], int n) {
    printf("DATA [");
    for (int i = 0; i < n; ++i) printf("%s%d", i ? " " : "", a[i]);
    puts("]");
}
#endif

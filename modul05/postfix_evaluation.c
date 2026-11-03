/* Contoh mandiri C11. Baca kontrak input dan kompleksitas di README.md. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

/* 1 = baris utuh, 0 = EOF, -1 = terlalu panjang; sisa baris dibuang.
   Tidak ada loop gagal scanf: setiap percobaan menghabiskan satu baris. */
static int read_line(char *s, size_t cap) {
    int c;
    size_t n;
    if (fgets(s, (int)cap, stdin) == NULL) return 0;
    n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') { s[n - 1] = '\0'; return 1; }
    c = getchar();
    if (c == '\n' || c == EOF) return 1;
    while (c != '\n' && c != EOF) c = getchar();
    return -1;
}

/* strtol mendeteksi overflow sebelum konversi ke int. */
static int integer(const char *s, int *value) {
    char *end;
    long v;
    if (*s == '\0') return 0;
    errno = 0;
    v = strtol(s, &end, 10);
    if (errno == ERANGE || *end != '\0' || v < INT_MIN || v > INT_MAX) return 0;
    *value = (int)v;
    return 1;
}
/* Token dipisahkan spasi; -1 berarti jumlah token melewati kapasitas. */
static int tokens(char *line, char **out, int cap) {
    int n = 0;
    char *p = strtok(line, " \t\r");
    while (p != NULL) {
        if (n == cap) return -1;
        out[n++] = p;
        p = strtok(NULL, " \t\r");
    }
    return n;
}

/* Postfix bertoken spasi: int bertanda atau operator biner + - * /.
   Tidak memakai double: pembagian int dipotong ke nol menurut C11.
   O(L) waktu termasuk parsing, O(L) ruang stack.
   Pemeriksaan SEBELUM operasi mencegah signed integer overflow. */
static int apply(int a, int b, char op, int *out) {
    if (op == '+') {
        if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) return 0;
        *out = a + b;
    } else if (op == '-') {
        if ((b > 0 && a < INT_MIN + b) || (b < 0 && a > INT_MAX + b)) return 0;
        *out = a - b;
    } else if (op == '*') {
        if (a > 0) {
            if ((b > 0 && a > INT_MAX / b) || (b < 0 && b < INT_MIN / a)) return 0;
        } else if (a < 0) {
            if ((b > 0 && a < INT_MIN / b) || (b < 0 && a < INT_MAX / b)) return 0;
        }
        *out = a * b;
    } else {
        if (b == 0 || (a == INT_MIN && b == -1)) return 0;
        *out = a / b;
    }
    return 1;
}
static int evaluate(char *line, int *out) {
    char *t[256];
    int stack[256], n = tokens(line, t, 256), top = 0, i;
    if (n <= 0) return 0;
    for (i = 0; i < n; ++i) {
        if (strlen(t[i]) == 1 && strchr("+-*/", t[i][0]) != NULL) {
            int b, a, value;
            if (top < 2) return 0;
            b = stack[--top]; a = stack[--top];
            if (!apply(a, b, t[i][0], &value)) return 0;
            stack[top++] = value;
        } else {
            int value;
            if (top == 256 || !integer(t[i], &value)) return 0;
            stack[top++] = value;
        }
    }
    if (top != 1) return 0;
    *out = stack[0]; return 1;
}
int main(void) {
    char line[512];
    int status, result;
    while ((status = read_line(line, sizeof line)) != 0) {
        if (status < 0 || !evaluate(line, &result)) puts("ERROR ekspresi");
        else printf("HASIL %d\n", result);
    }
    return 0;
}

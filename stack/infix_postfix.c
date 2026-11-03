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

/* Grammar: satu huruf ASCII atau satu digit per operand; operator biner + - * /;
   tanda kurung dan spasi diizinkan, TANPA unary, pangkat, atau bilangan multi-digit.
   Shunting-yard: tiap simbol push/pop sekali -> O(L), ruang O(L).
   need_operand mencegah AB, A++, (), dan tanda kurung implisit. */
static int letter(unsigned char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
static int precedence(char c) { return (c == '*' || c == '/') ? 2 : 1; }
static int convert(const char *s, char *out) {
    char stack[512];
    size_t top = 0, used = 0, i;
    int need_operand = 1;
    for (i = 0; s[i] != '\0'; ++i) {
        unsigned char c = (unsigned char)s[i];
        if (isspace(c)) continue;
        if (letter(c) || (c >= '0' && c <= '9')) {
            if (!need_operand) return 0;
            out[used++] = (char)c; out[used++] = ' '; need_operand = 0;
        } else if (c == '(') {
            if (!need_operand || top == sizeof stack) return 0;
            stack[top++] = '(';
        } else if (c == ')') {
            if (need_operand) return 0;
            while (top > 0 && stack[top - 1] != '(') {
                out[used++] = stack[--top]; out[used++] = ' ';
            }
            if (top == 0) return 0;
            --top;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (need_operand) return 0;
            while (top > 0 && stack[top - 1] != '(' && precedence(stack[top - 1]) >= precedence((char)c)) {
                out[used++] = stack[--top]; out[used++] = ' ';
            }
            if (top == sizeof stack) return 0;
            stack[top++] = (char)c; need_operand = 1;
        } else return 0;
    }
    if (need_operand) return 0;
    while (top > 0) {
        char c = stack[--top];
        if (c == '(') return 0;
        out[used++] = c; out[used++] = ' ';
    }
    /* Maksimal 511 simbol input; setiap simbol output memakai <=2 byte. */
    out[used - 1] = '\0';
    return 1;
}
int main(void) {
    char line[512], result[1024];
    int status;
    while ((status = read_line(line, sizeof line)) != 0) {
        if (status < 0 || !convert(line, result)) puts("ERROR ekspresi");
        else printf("POSTFIX %s\n", result);
    }
    return 0;
}

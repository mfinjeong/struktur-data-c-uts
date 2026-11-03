/* PRD - Tujuan: latihan asli Modul 5/6, C11, array 1D atau SLL saja.
 * PRD - Input: satu perintah/ekspresi per baris; EOF mengakhiri program.
 * PRD - Keamanan: validasi batas/input sebelum mutasi; memori selalu dibebaskan.
 */
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

/* Praktikum 5.6: infix digit tunggal -> postfix -> evaluasi int C11.
   Input: ekspresi per baris, biner + - * /, kurung (), spasi.
   O(L) waktu dan ruang; tiap simbol push/pop sekali. Pembagian menuju nol.
   DEPTH = maksimum gabungan stack kurung, operator, dan nilai evaluasi,
   seperti pencacah bersama praktikum (bukan jumlah ketiganya).
   Output hanya setelah semua tahap valid; overflow/zero ditolak. */
static int precedence(char c) { return (c == '*' || c == '/') ? 2 : 1; }
static int convert(const char *s, char *out, int *depth) {
    char stack[512];
    size_t top = 0, used = 0, i;
    int need_operand = 1;
    for (i = 0; s[i] != '\0'; ++i) {
        unsigned char c = (unsigned char)s[i];
        if (isspace(c)) continue;
        if (c >= '0' && c <= '9') {
            if (!need_operand) return 0;
            out[used++] = (char)c; out[used++] = ' '; need_operand = 0;
        } else if (c == '(') {
            if (!need_operand || top == sizeof stack) return 0;
            stack[top++] = '(';
            if ((int)top > *depth) *depth=(int)top;
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
            if ((int)top > *depth) *depth=(int)top;
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
static int evaluate(const char *s, int *out, int *depth) {
    int values[512]; size_t top=0;
    for (;*s;++s) {
        if (*s==' ') continue;
        if (*s>='0' && *s<='9') {
            if (top==512) return 0;
            values[top++]=*s-'0';
            if ((int)top>*depth) *depth=(int)top;
        } else {
            int a,b,value;
            if (top<2) return 0;
            b=values[--top]; a=values[--top];
            if (!apply(a,b,*s,&value)) return 0;
            values[top++]=value;
        }
    }
    if (top!=1) return 0;
    *out=values[0]; return 1;
}
static int bracket_depth(const char *s) {
    int now=0, peak=0;
    for (;*s;++s) {
        if (*s=='(') { ++now; if (now>peak) peak=now; }
        else if (*s==')') --now;
    }
    return peak;
}
int main(void) {
    char line[512], result[1024]; int status;
    while ((status=read_line(line,sizeof line))!=0) {
        int value,depth=bracket_depth(line);
        if (status<0 || !convert(line,result,&depth) || !evaluate(result,&value,&depth)) puts("ERROR ekspresi");
        else printf("POSTFIX %s\nHASIL %d\nDEPTH %d\n",result,value,depth);
    }
    return 0;
}

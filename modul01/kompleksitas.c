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

/* Menghitung banyak operasi, bukan menghabiskan waktu dengan n*n iterasi.
   Konstan O(1), linear O(n), pasangan bersarang O(n^2); ruang O(1).
   Input dibatasi 0..1000000 agar perkalian long long aman. */
int main(void) {
    char line[512], *t[2];
    int status, n;
    while ((status = read_line(line, sizeof line)) != 0) {
        if (status < 0 || tokens(line, t, 2) != 1 ||
            !integer(t[0], &n) || n < 0 || n > 1000000) {
            puts("ERROR input"); continue;
        }
        printf("n=%d konstan=1 linear=%d kuadrat=%lld\n", n, n, (long long)n * n);
    }
    return 0;
}

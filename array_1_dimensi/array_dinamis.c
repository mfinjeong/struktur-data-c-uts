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

/* Invarian: 0 <= n <= cap; elemen sah hanya [0,n).
   Akses/update O(1), sisip/hapus O(n). Pencetakan O(n).
   Dinamis: append amortisasi O(1), terburuk O(n) ketika realloc;
   ruang O(cap). Statis: append O(1), ruang tetap delapan int. */
int main(void) {
    int *a = NULL; size_t n = 0, cap = 0;
    char line[512], *t[4];
    int status;
    while ((status = read_line(line, sizeof line)) != 0) {
        int k, x = 0, y = 0;
        if (status < 0) { puts("ERROR baris panjang"); continue; }
        k = tokens(line, t, 4);
        if (k == 0) continue;
        if (k < 0 || (k > 1 && !integer(t[1], &x)) ||
            (k > 2 && !integer(t[2], &y))) { puts("ERROR perintah"); continue; }
        if (k == 1 && strcmp(t[0], "quit") == 0) break;

        if (k == 1 && strcmp(t[0], "print") == 0) {
            size_t i;
            printf("DATA [");
            for (i = 0; i < n; ++i) printf("%s%d", i ? " " : "", a[i]);
            puts("]");
        } else if ((k == 2 && strcmp(t[0], "append") == 0) ||
                   (k == 3 && strcmp(t[0], "insert") == 0)) {
            size_t pos, i;
            int value;
            if (k == 2) { pos = n; value = x; }
            else {
                if (x < 0 || (size_t)x > n) { puts("ERROR indeks"); continue; }
                pos = (size_t)x; value = y;
            }
            if (n == cap) {
                /* realloc gagal tidak menghilangkan pointer lama.
                   Batas 1 juta elemen menjaga aritmetika ukuran tetap aman. */
                size_t next = cap == 0 ? 2 : cap * 2;
                int *temp;
                if (next > 1000000) { puts("ERROR kapasitas"); continue; }
                temp = realloc(a, next * sizeof *a);
                if (temp == NULL) { puts("ERROR memori"); continue; }
                a = temp; cap = next;
            }
            /* Geser dari kanan agar elemen lama tidak tertimpa. */
            for (i = n; i > pos; --i) a[i] = a[i - 1];
            a[pos] = value; ++n;
            puts("OK");
        } else if (k == 3 && strcmp(t[0], "set") == 0) {
            if (x < 0 || (size_t)x >= n) { puts("ERROR indeks"); continue; }
            a[x] = y; puts("OK");
        } else if (k == 2 && strcmp(t[0], "erase") == 0) {
            size_t i;
            if (x < 0 || (size_t)x >= n) { puts("ERROR indeks"); continue; }
            for (i = (size_t)x; i + 1 < n; ++i) a[i] = a[i + 1];
            --n; puts("OK");
        } else puts("ERROR perintah");
    }
    free(a);
    return 0;
}

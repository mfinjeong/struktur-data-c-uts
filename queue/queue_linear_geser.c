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

/* Varian tambahan BERGESER, bukan linear front/rear Modul 6.3.
   FIFO linear: elemen aktif selalu a[0..n). Dequeue menggeser kiri O(n)
   (best O(1) saat satu elemen); enqueue/peek O(1), print O(n).
   Ruang tetap O(8); slot bekas langsung dapat digunakan kembali. */
int main(void) {
    int a[8], n = 0;
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

        (void)y;
        if (k == 2 && !strcmp(t[0], "enqueue")) {
            if (n == 8) puts("ERROR penuh");
            else { a[n] = x; ++n; puts("OK"); }
        } else if (k == 1 && !strcmp(t[0], "dequeue")) {
            if (n == 0) puts("ERROR kosong");
            else {
                printf("DEQUEUE %d\n", a[0]);
                int i;
                for (i = 1; i < n; ++i) a[i - 1] = a[i];
                --n;
            }
        } else if (k == 1 && !strcmp(t[0], "peek")) {
            if (n == 0) puts("ERROR kosong");
            else printf("FRONT %d\n", a[0]);
        } else if (k == 1 && !strcmp(t[0], "print")) {
            int i;
            printf("DATA [");
            for (i = 0; i < n; ++i) printf("%s%d", i ? " " : "", a[i]);
            puts("]");
        } else puts("ERROR perintah");
    }
    return 0;
}

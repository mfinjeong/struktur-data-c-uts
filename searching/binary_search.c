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

/* Satu baris: n lalu tepat n bilangan; 0 <= n <= 128.
   Array tetap menghindari VLA, tetapi panjang logis boleh nol. */
static int read_array(int a[128], int *n) {
    char line[4096], *t[130];
    int k, i;
    if (read_line(line, sizeof line) != 1) return 0;
    k = tokens(line, t, 130);
    if (k < 1 || !integer(t[0], n) || *n < 0 || *n > 128 || k != *n + 1) return 0;
    for (i = 0; i < *n; ++i) if (!integer(t[i + 1], &a[i])) return 0;
    return 1;
}

/* Linear: terbaik O(1), rata-rata/terburuk O(n), ruang O(1).
   Binary lower_bound: terbaik/rata-rata/terburuk O(log n) untuk n>1,
   ruang O(1); validasi urutan awal O(n), tidak diulang setiap kueri. */
int main(void) {
    int a[128], n;
    char line[512], *t[2];
    int status;
    if (!read_array(a, &n)) { puts("ERROR input"); return 0; }
    {
        int i;
        for (i = 1; i < n; ++i) if (a[i - 1] > a[i]) { puts("ERROR urutan"); return 0; }
    }
    while ((status = read_line(line, sizeof line)) != 0) {
        int target, found = -1;
        if (status < 0 || tokens(line, t, 2) != 1 || !integer(t[0], &target)) {
            puts("ERROR input"); continue;
        }
        int lo = 0, hi = n;
        /* Interval kandidat [lo,hi); lower_bound memilih duplikat pertama.
           Tengah dihitung tanpa lo+hi yang dapat overflow. */
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] < target) lo = mid + 1;
            else hi = mid;
        }
        if (lo < n && a[lo] == target) found = lo;
        printf("INDEX %d\n", found);
    }
    return 0;
}

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

/* Perbandingan langsung, bukan a-b (pengurangan bisa overflow).
   desc mengubah arah, tidak mengubah algoritme. */
static int before(int a, int b, int desc) { return desc ? a > b : a < b; }
static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }
/* Bubble stabil: best O(n) (early stop), avg/worst O(n^2), ruang O(1). */
static void bubble(int *a, int n, int d) {
    int end;
    for (end = n - 1; end > 0; --end) {
        int i, changed = 0;
        for (i = 0; i < end; ++i) if (before(a[i + 1], a[i], d)) {
            swap(&a[i], &a[i + 1]); changed = 1;
        }
        if (!changed) break;
    }
}
/* Selection tidak stabil: semua kasus O(n^2), ruang O(1).
   Prefix [0,i) sudah final. */
static void selection(int *a, int n, int d) {
    int i;
    for (i = 0; i < n; ++i) {
        int j, best = i;
        for (j = i + 1; j < n; ++j) if (before(a[j], a[best], d)) best = j;
        swap(&a[i], &a[best]);
    }
}
/* Insertion stabil: best O(n), avg/worst O(n^2), ruang O(1).
   Prefix [0,i) terurut; geser hanya elemen yang benar-benar mendahului key. */
static void insertion(int *a, int n, int d) {
    int i;
    for (i = 1; i < n; ++i) {
        int key = a[i], j = i;
        while (j > 0 && before(key, a[j - 1], d)) { a[j] = a[j - 1]; --j; }
        a[j] = key;
    }
}
/* Merge stabil: semua kasus O(n log n), buffer O(n), stack O(log n).
   Interval setengah terbuka [lo,hi) aman juga untuk n=0. */
static void merge_sort(int *a, int *tmp, int lo, int hi, int d) {
    int mid, i, j, k;
    if (hi - lo < 2) return;
    mid = lo + (hi - lo) / 2;
    merge_sort(a, tmp, lo, mid, d); merge_sort(a, tmp, mid, hi, d);
    i = lo; j = mid; k = lo;
    while (i < mid && j < hi) {
        if (before(a[j], a[i], d)) tmp[k++] = a[j++];
        else tmp[k++] = a[i++]; /* Sama: ambil kiri agar stabil. */
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    for (k = lo; k < hi; ++k) a[k] = tmp[k];
}
/* Quick Lomuto: pivot terakhir; tidak stabil. Best/avg O(n log n),
   worst O(n^2) pada data terurut/duplikat. Stack avg O(log n), worst O(n).
   Batas input 128 menjaga kedalaman rekursi contoh ini. */
static void quick(int *a, int lo, int hi, int d) {
    int i, j, pivot;
    if (lo >= hi) return;
    pivot = a[hi]; i = lo;
    for (j = lo; j < hi; ++j) if (before(a[j], pivot, d)) { swap(&a[i], &a[j]); ++i; }
    swap(&a[i], &a[hi]);
    quick(a, lo, i - 1, d); quick(a, i + 1, hi, d);
}
int main(void) {
    char line[512], *t[3], algorithm[16];
    int a[128], tmp[128], n, d, i;
    if (read_line(line, sizeof line) != 1 || tokens(line, t, 3) != 2) {
        puts("ERROR input"); return 0;
    }
    if (strlen(t[0]) >= sizeof algorithm) { puts("ERROR algoritme"); return 0; }
    strcpy(algorithm, t[0]);
    if (strcmp(t[1], "asc") != 0 && strcmp(t[1], "desc") != 0) {
        puts("ERROR arah"); return 0;
    }
    d = strcmp(t[1], "desc") == 0;
    if (strcmp(algorithm, "bubble") && strcmp(algorithm, "selection") &&
        strcmp(algorithm, "insertion") && strcmp(algorithm, "merge") && strcmp(algorithm, "quick")) {
        puts("ERROR algoritme"); return 0;
    }
    if (!read_array(a, &n)) { puts("ERROR input"); return 0; }
    if (!strcmp(algorithm, "bubble")) bubble(a, n, d);
    else if (!strcmp(algorithm, "selection")) selection(a, n, d);
    else if (!strcmp(algorithm, "insertion")) insertion(a, n, d);
    else if (!strcmp(algorithm, "merge")) merge_sort(a, tmp, 0, n, d);
    else quick(a, 0, n - 1, d);
    printf("DATA [");
    for (i = 0; i < n; ++i) printf("%s%d", i ? " " : "", a[i]);
    puts("]");
    return 0;
}

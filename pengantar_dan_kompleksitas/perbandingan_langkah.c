/* Rujukan konsep: 1.5, 1.6 dan Praktikum 1 langkah 3-5.
   Tanpa input. Langkah = satu elemen diperiksa, bukan setiap operator C.
   Data sudah terurut; biaya pembangkitan tidak dihitung sebagai pencarian. */
#include <stdio.h>
#include <stdlib.h>
static int sequential(const int a[], int n, int key, size_t *steps) {
    *steps = 0;
    for (int i = 0; i < n; ++i) {
        ++*steps;
        if (a[i] == key) return i;
    }
    return -1;
}
static int binary(const int a[], int n, int key, size_t *steps) {
    int left = 0, right = n - 1; *steps = 0;
    while (left <= right) {
        int mid = left + (right - left) / 2; ++*steps;
        if (a[mid] == key) return mid;
        if (a[mid] < key) left = mid + 1; else right = mid - 1;
    }
    return -1;
}
int main(void) {
    const int sizes[] = {1000, 10000, 100000, 1000000};
    puts("n sequential binary");
    for (size_t u = 0; u < sizeof sizes / sizeof sizes[0]; ++u) {
        int n = sizes[u]; int *a = malloc((size_t)n * sizeof *a);
        if (!a) { puts("ERROR alokasi"); return 1; }
        for (int i = 0; i < n; ++i) a[i] = 2 * i;
        size_t s, b;
        int x = sequential(a, n, a[n-1], &s);
        int y = binary(a, n, a[n-1], &b);
        free(a);
        if (x != n-1 || y != n-1) return 1;
        printf("%d %zu %zu\n", n, s, b);
    }
    return 0;
}

/* Konsep 3.2-3.4. Input n (0..128), n int; output menaik.
   Mandiri: cc insertion_sort.c -o insertion_sort
   SORT_LIBRARY dipakai pembanding untuk menggunakan implementasi yang sama. */
#include "statistik_sorting.h"
void insertion_sort(int a[], int n, Stats *s) {
    for (int i = 1; i < n; ++i) {
        int key = a[i], j = i;
        while (j > 0) {
            ++s->banding;
            if (a[j-1] <= key) break;
            a[j] = a[j-1]; --j; ++s->geser;
        }
        a[j] = key;
    }
}
#ifndef SORT_LIBRARY
#include "input_array.h"
int main(void) {
    int a[128], n; Stats s = {0,0,0};
    if (!baca_array(a, &n) || !selesai_input()) { puts("ERROR input"); return 0; }
    insertion_sort(a, n, &s);
    tampil_array(a, n);
    return 0;
}
#endif

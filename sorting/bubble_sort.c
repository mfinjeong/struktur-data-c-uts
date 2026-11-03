/* Konsep 3.2-3.4. Input n (0..128), n int; output menaik.
   Mandiri: cc bubble_sort.c -o bubble_sort
   SORT_LIBRARY dipakai pembanding untuk menggunakan implementasi yang sama. */
#include "statistik_sorting.h"
void bubble_sort(int a[], int n, Stats *s) {
    for (int end = n-1; end > 0; --end) {
        int changed = 0;
        for (int j = 0; j < end; ++j) {
            ++s->banding;
            if (a[j] > a[j+1]) { tukar(&a[j], &a[j+1], s); changed = 1; }
        }
        if (!changed) break;
    }
}
#ifndef SORT_LIBRARY
#include "input_array.h"
int main(void) {
    int a[128], n; Stats s = {0,0,0};
    if (!baca_array(a, &n) || !selesai_input()) { puts("ERROR input"); return 0; }
    bubble_sort(a, n, &s);
    tampil_array(a, n);
    return 0;
}
#endif

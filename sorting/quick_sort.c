/* Konsep 3.2-3.4. Input n (0..128), n int; output menaik.
   Mandiri: cc quick_sort.c -o quick_sort
   SORT_LIBRARY dipakai pembanding untuk menggunakan implementasi yang sama. */
#include "statistik_sorting.h"
/* Pivot terakhir memperlihatkan kasus buruk O(n^2) pada data terurut.
   Rekursi hanya sisi lebih pendek; sisi panjang lewat loop: stack O(log n).
   Tukar termasuk self-swap, sesuai penghitung praktikum 3. */
static void quick_range(int a[], int lo, int hi, Stats *s) {
    while (lo < hi) {
        int pivot = a[hi], p = lo;
        for (int j = lo; j < hi; ++j) {
            ++s->banding;
            if (a[j] <= pivot) { tukar(&a[p], &a[j], s); ++p; }
        }
        tukar(&a[p], &a[hi], s);
        if (p-lo < hi-p) { quick_range(a, lo, p-1, s); lo = p+1; }
        else { quick_range(a, p+1, hi, s); hi = p-1; }
    }
}
void quick_sort(int a[], int n, Stats *s) { quick_range(a, 0, n-1, s); }
#ifndef SORT_LIBRARY
#include "input_array.h"
int main(void) {
    int a[128], n; Stats s = {0,0,0};
    if (!baca_array(a, &n) || !selesai_input()) { puts("ERROR input"); return 0; }
    quick_sort(a, n, &s);
    tampil_array(a, n);
    return 0;
}
#endif

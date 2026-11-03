/* Konsep 3.2-3.4. Input n (0..128), n int; output menaik.
   Mandiri: cc merge_sort.c -o merge_sort
   SORT_LIBRARY dipakai pembanding untuk menggunakan implementasi yang sama. */
#include "statistik_sorting.h"
/* Rentang setengah terbuka [lo, hi); buffer tetap sesuai batas input.
   Ambil kiri ketika sama agar stabil; O(n log n), ruang O(n). */
static void gabung_urut(int a[], int tmp[], int lo, int hi, Stats *s) {
    if (hi-lo < 2) return;
    int mid = lo+(hi-lo)/2;
    gabung_urut(a, tmp, lo, mid, s); gabung_urut(a, tmp, mid, hi, s);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        ++s->banding;
        tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++];
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    for (k = lo; k < hi; ++k) a[k] = tmp[k];
}
void merge_sort(int a[], int n, Stats *s) {
    int tmp[128]; gabung_urut(a, tmp, 0, n, s);
}
#ifndef SORT_LIBRARY
#include "input_array.h"
int main(void) {
    int a[128], n; Stats s = {0,0,0};
    if (!baca_array(a, &n) || !selesai_input()) { puts("ERROR input"); return 0; }
    merge_sort(a, n, &s);
    tampil_array(a, n);
    return 0;
}
#endif

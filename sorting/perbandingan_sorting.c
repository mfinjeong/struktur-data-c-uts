/* Praktikum 3: empat algoritme inti + merge dari teori 3.3.
   Tanpa input; n=128, tiga pola. LCG uint32_t membuat acak lintas platform.
   File algoritme dimasukkan sebagai library (main dinonaktifkan), bukan
   menyalin lima implementasi ke setiap program. Kompilasi file ini saja.
   Tidak mengarang waktu; hanya operasi nyata yang dicacah. */
#include <stdio.h>
#include <stdint.h>
#define SORT_LIBRARY
#include "bubble_sort.c"
#include "selection_sort.c"
#include "insertion_sort.c"
#include "merge_sort.c"
#include "quick_sort.c"
int main(void) {
    const char *pola[] = {"acak", "terurut", "terbalik"};
    const char *nama[] = {"bubble", "selection", "insertion", "merge", "quick"};
    void (*fungsi[])(int *, int, Stats *) = {bubble_sort, selection_sort, insertion_sort, merge_sort, quick_sort};
    uint32_t seed = UINT32_C(12345);
    for (int p = 0; p < 3; ++p) {
        int asal[128];
        for (int i = 0; i < 128; ++i) {
            seed = seed * UINT32_C(1664525) + UINT32_C(1013904223);
            asal[i] = p == 0 ? (int)(seed % 1280u) : (p == 1 ? i : 128-i);
        }
        printf("POLA %s n=128\n", pola[p]);
        for (int f = 0; f < 5; ++f) {
            int a[128]; Stats s = {0,0,0};
            for (int i = 0; i < 128; ++i) a[i] = asal[i];
            fungsi[f](a, 128, &s);
            for (int i = 1; i < 128; ++i) if (a[i-1] > a[i]) return 1;
            printf("%s banding=%zu tukar=%zu geser=%zu benar\n", nama[f], s.banding, s.tukar, s.geser);
        }
    }
    return 0;
}

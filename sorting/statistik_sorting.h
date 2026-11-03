#ifndef STATISTIK_SORTING_H
#define STATISTIK_SORTING_H
#include <stddef.h>
/* banding: perbandingan nilai, bukan cek batas loop.
   tukar: panggilan swap; geser: perpindahan insertion, bukan swap.
   Penyalinan merge tidak termasuk tukar/geser (nol bukan tanpa kerja). */
typedef struct { size_t banding, tukar, geser; } Stats;
static inline void tukar(int *a, int *b, Stats *s) {
    int tmp = *a; *a = *b; *b = tmp; ++s->tukar;
}
#endif

/* Konsep 2.1-2.2: ukuran tidak diasumsikan 4 byte; alamat bukan angka tetap.
   Tanpa input. sizeof array hanya dipakai di lingkup deklarasinya.
   %p membutuhkan void*, %zu untuk size_t. */
#include <stdio.h>
static double rerata(const int data[], size_t n) {
    double total = 0;
    for (size_t i = 0; i < n; ++i) total += data[i];
    return n ? total / (double)n : 0.0;
}
int main(void) {
    int nilai[] = {15, 27, 34, 46, 58}, kuadrat[5];
    size_t n = sizeof nilai / sizeof nilai[0];
    puts("DATA [15 27 34 46 58]");
    printf("KUADRAT [");
    for (size_t i = 0; i < n; ++i) {
        kuadrat[i] = (int)((i + 1) * (i + 1));
        printf("%s%d", i ? " " : "", kuadrat[i]);
    }
    puts("]");
    for (size_t i = 0; i < n; ++i)
        printf("INDEKS %zu NILAI %d ALAMAT %p OFFSET_BYTE %zu\n", i, nilai[i], (void *)&nilai[i], i * sizeof nilai[0]);
    printf("ELEMEN_BYTE %zu ARRAY_BYTE %zu\n", sizeof nilai[0], sizeof nilai);
    printf("JUMLAH %zu\nRERATA %.2f\n", n, rerata(nilai, n));
    return 0;
}
